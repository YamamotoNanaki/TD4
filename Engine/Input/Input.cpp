#include "Input.h"
#include "WindowsAPI.h"
#include "Debug.h"
#include <cassert>

using namespace IFE;

#pragma comment (lib, "xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::sInputInstance_ = nullptr;
bool Input::sPadConnected_ = false;

Input* IFE::Input::Instance()
{
	static Input inst;
	return &inst;
}

void IFE::Input::Initalize()
{
	sInputInstance_ = Instance();
	HWND hwnd = *WindowsAPI::Instance()->GetHWnd();

	HRESULT result = DirectInput8Create(
		*WindowsAPI::Instance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&sInputInstance_->directInput_, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	result = sInputInstance_->directInput_->CreateDevice(GUID_SysKeyboard, &sInputInstance_->keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = sInputInstance_->keyboard_->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = sInputInstance_->keyboard_->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//マウスデバイスの生成
	result = sInputInstance_->directInput_->CreateDevice(GUID_SysMouse, &sInputInstance_->devMouse_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = sInputInstance_->devMouse_->SetDataFormat(&c_dfDIMouse2); // 標準形式
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = sInputInstance_->devMouse_->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//コントローラデバイスの生成
	XInputGetState(
		0,       // DWORD         dwUserIndex
		&sInputInstance_->pad_); // XINPUT_STATE* pState
}

void IFE::Input::Update()
{
	HRESULT result = S_OK;
	for (int32_t i = 0; i < 256; i++)
	{
		sInputInstance_->oldkey_[i] = sInputInstance_->key_[i];
	}
	sInputInstance_->keyboard_->Acquire();
	result = sInputInstance_->keyboard_->GetDeviceState(sizeof(sInputInstance_->key_), sInputInstance_->key_);

	// 前回の入力を保存
	sInputInstance_->oldmouse_ = sInputInstance_->mouse_;
	// マウス
	result = sInputInstance_->devMouse_->Acquire();	// マウス動作開始
	// マウスの入力
	result = sInputInstance_->devMouse_->GetDeviceState(sizeof(sInputInstance_->mouse_), &sInputInstance_->mouse_);

	sInputInstance_->oldpad_ = sInputInstance_->pad_;
	DWORD dwResult;
	dwResult = XInputGetState(
		0,       // DWORD         dwUserIndex
		&sInputInstance_->pad_); // XINPUT_STATE* pState
	if (dwResult == ERROR_SUCCESS)
	{
		sPadConnected_ = true;
	}
	else
	{
		sPadConnected_ = false;
	}
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(*WindowsAPI::Instance()->GetHWnd(), &p);
	sMousePoint_ = { (float)p.x,(float)p.y };
}

bool Input::GetMousePush(const Mouse& m)
{
	if (sInputInstance_->mouse_.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

bool Input::GetMouseTrigger(const Mouse& m)
{
	if (sInputInstance_->mouse_.rgbButtons[(BYTE)m] && !sInputInstance_->oldmouse_.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

bool Input::GetMouseRelease(const Mouse& m)
{
	if (!sInputInstance_->mouse_.rgbButtons[(BYTE)m] && sInputInstance_->oldmouse_.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

Float2 Input::GetLAnalog(int32_t unresponsive_range)
{
	return Float2(GetLXAnalog(unresponsive_range), GetLYAnalog(unresponsive_range));
}
Float2 Input::GetRAnalog(int32_t unresponsive_range)
{
	return Float2(GetRXAnalog(unresponsive_range), GetRYAnalog(unresponsive_range));
}
float Input::GetLXAnalog(int32_t unresponsive_range)
{
	float x = sInputInstance_->pad_.Gamepad.sThumbLX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}
float Input::GetRXAnalog(int32_t unresponsive_range)
{
	float x = sInputInstance_->pad_.Gamepad.sThumbRX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}
float Input::GetLYAnalog(int32_t unresponsive_range)
{
	float y = sInputInstance_->pad_.Gamepad.sThumbLY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}
float Input::GetRYAnalog(int32_t unresponsive_range)
{
	float y = sInputInstance_->pad_.Gamepad.sThumbRY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}
bool Input::PadPush(const PADCODE& p)
{
	if (sInputInstance_->pad_.Gamepad.wButtons & (uint32_t)p)return true;
	return false;
}
bool Input::PadTrigger(const PADCODE& p)
{
	if ((sInputInstance_->pad_.Gamepad.wButtons & (uint32_t)p) && !(sInputInstance_->oldpad_.Gamepad.wButtons & (uint32_t)p))
		return true;
	return false;
}
bool Input::PadRelease(const PADCODE& p)
{
	if (!(sInputInstance_->pad_.Gamepad.wButtons & (uint32_t)p) && (sInputInstance_->oldpad_.Gamepad.wButtons & (uint32_t)p))return true;
	return false;
}
void Input::PadVibrationStart(const WORD& L, const WORD& R)
{
	sInputInstance_->vibration_.wLeftMotorSpeed = L;
	sInputInstance_->vibration_.wRightMotorSpeed = R;
	XInputSetState(0, &sInputInstance_->vibration_);
}
void Input::PadVibrationStop()
{
	sInputInstance_->vibration_.wLeftMotorSpeed = 0;
	sInputInstance_->vibration_.wRightMotorSpeed = 0;
	XInputSetState(0, &sInputInstance_->vibration_);
}

bool IFE::Input::GetPadConnected()
{
	return sPadConnected_;
}

bool IFE::Input::GetKeyTrigger(const Key& keyFlag)
{
	return  sInputInstance_->key_[(uint16_t)keyFlag] && !sInputInstance_->oldkey_[(uint16_t)keyFlag];
}

bool IFE::Input::GetKeyPush(const Key& keyFlag)
{
	return sInputInstance_->key_[(uint16_t)keyFlag];
}

bool IFE::Input::GetKeyRelease(const Key& keyFlag)
{
	return  !sInputInstance_->key_[(uint16_t)keyFlag] && sInputInstance_->oldkey_[(uint16_t)keyFlag];
}

Float2 IFE::Input::GetMousePoint()
{
	return sMousePoint_;
}

int32_t IFE::Input::GetMouseWheel()
{
	return sInputInstance_->mouse_.lZ;
}

float IFE::Input::GetRTrigger(std::int32_t unresponsive_range)
{
	float r;
	r = sInputInstance_->pad_.Gamepad.bRightTrigger;
	if (r < unresponsive_range)
	{
		r = 0;
	}
	return r;
}

float IFE::Input::GetLTrigger(std::int32_t unresponsive_range)
{
	float l;
	l = sInputInstance_->pad_.Gamepad.bLeftTrigger;
	if (l < unresponsive_range)
	{
		l = 0;
	}
	return l;
}

Float2 IFE::Input::GetMousePointScalar()
{
	return Float2(float(sInputInstance_->mouse_.lX), float(sInputInstance_->mouse_.lY));
}
