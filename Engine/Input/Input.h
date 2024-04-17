#pragma once
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include <cstdint>
#include "Float2.h"

namespace IFE
{
	enum class Key;

	enum class PADCODE;

	enum class Mouse;

	class Input final
	{
	private:
		char key_[256]{}, oldkey_[256]{};
		Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> devMouse_ = nullptr;
		DIMOUSESTATE2 mouse_ = {};
		DIMOUSESTATE2 oldmouse_ = {};
		XINPUT_STATE pad_{};
		XINPUT_STATE oldpad_{};
		XINPUT_VIBRATION vibration_{};
		static Input* sInputInstance_;
		static bool sPadConnected_;
		inline static Float2 sMousePoint_;
	private:
		Input() {}
		Input(const Input&) {}
		Input& operator=(const Input&) {}
		~Input() {}

	public:
		static Input* Instance();
		static void Initalize();

		/// <summary>
		/// �L�[�{�[�h���̃A�b�v�f�[�g
		/// </summary>
		/// <param name="result"></param>
		static void Update();

		//�L�[�{�[�h�̔���
		/// <summary>
		/// �g���K�[����
		/// </summary>
		/// <param name="keyCode">�������肽���L�[�̃}�N��</param>
		/// <returns></returns>
		static bool GetKeyTrigger(const Key& keyFlag);
		/// <summary>
		/// ���͔���
		/// </summary>
		/// <param name="keyCode">�������肽���L�[�̃}�N��</param>
		/// <returns></returns>
		static bool GetKeyDown(const Key& keyFlag);
		/// <summary>
		/// �����[�X����
		/// </summary>
		/// <param name="keyCode">�������肽���L�[�̃}�N��</param>
		/// <returns></returns>
		static bool GetKeyRelease(const Key& keyFlag);
		//�}�E�X�̔���
		static bool GetMousePush(const Mouse& mouse);
		static bool GetMouseTrigger(const Mouse& mouse);
		static bool GetMouseRelease(const Mouse& mouse);
		static Float2 GetMousePoint();
		static int32_t GetMouseWheel();
		//�R���g���[���[�̔���
		static Float2 GetLAnalog(std::int32_t unresponsive_range = 1000);
		static Float2 GetRAnalog(std::int32_t unresponsive_range = 1000);
		static float GetLXAnalog(std::int32_t unresponsive_range = 1000);
		static float GetRXAnalog(std::int32_t unresponsive_range = 1000);
		static float GetLYAnalog(std::int32_t unresponsive_range = 1000);
		static float GetRYAnalog(std::int32_t unresponsive_range = 1000);
		static bool PadDown(const PADCODE& pad);
		static bool PadTriggere(const PADCODE& pad);
		static bool PadRelease(const PADCODE& pad);
		static void PadVibrationStart(const WORD& L, const WORD& R);
		static void PadVibrationStop();
		static bool GetPadConnected();
	};

	enum class Mouse
	{
		Left = 0b00, Right = 0b01, Middle = 0b10
	};

	enum class Key
	{
		BACK = DIK_BACK,	// BackSpace�L�[	D_DIK_BACK							
		TAB = DIK_TAB,	// Tab�L�[			D_DIK_TAB							
		RETURN = DIK_RETURN,	// Enter�L�[		D_DIK_RETURN						
		Enter = DIK_RETURN,	// Enter�L�[		D_DIK_RETURN						
		LSHIFT = DIK_LSHIFT,	// ��Shift�L�[		D_DIK_LSHIFT						
		RSHIFT = DIK_RSHIFT,	// �EShift�L�[		D_DIK_RSHIFT						
		LCONTROL = DIK_LCONTROL,	// ��Ctrl�L�[		D_DIK_LCONTROL						
		RCONTROL = DIK_RCONTROL,	// �ECtrl�L�[		D_DIK_RCONTROL						
		Esc = DIK_ESCAPE,	// Esc�L�[			D_DIK_ESCAPE						
		Space = DIK_SPACE,	// �X�y�[�X�L�[		D_DIK_SPACE							
		PGUP = DIK_PGUP,	// PageUp�L�[		D_DIK_PGUP							
		PGDN = DIK_PGDN,	// PageDown�L�[		D_DIK_PGDN							
		END = DIK_END,	// End�L�[			D_DIK_END							
		HOME = DIK_HOME,	// Home�L�[			D_DIK_HOME							
		LEFT = DIK_LEFT,	// ���L�[			D_DIK_LEFT							
		UP = DIK_UP,	// ��L�[			D_DIK_UP							
		RIGHT = DIK_RIGHT,	// �E�L�[			D_DIK_RIGHT							
		DOWN = DIK_DOWN,	// ���L�[			D_DIK_DOWN							
		INSERT = DIK_INSERT,	// Insert�L�[		D_DIK_INSERT						
		DELETE_Key = DIK_DELETE,	// Delete�L�[		D_DIK_DELETE						
		MINUS = DIK_MINUS,	// �|�L�[			D_DIK_MINUS							
		YEN = DIK_YEN,	// ���L�[			D_DIK_YEN							
		PREVTRACK = DIK_PREVTRACK,	// �O�L�[			D_DIK_PREVTRACK						
		PERIOD = DIK_PERIOD,	// �D�L�[			D_DIK_PERIOD						
		SLASH = DIK_SLASH,	// �^�L�[			D_DIK_SLASH							
		LALT = DIK_LALT,	// ��Alt�L�[		D_DIK_LALT							
		RALT = DIK_RALT,	// �EAlt�L�[		D_DIK_RALT							
		SCROLL = DIK_SCROLL,	// ScrollLock�L�[	D_DIK_SCROLL						
		SEMICOLON = DIK_SEMICOLON,	// �G�L�[			D_DIK_SEMICOLON						
		COLON = DIK_COLON,	// �F�L�[			D_DIK_COLON							
		LBRACKET = DIK_LBRACKET,	// �m�L�[			D_DIK_LBRACKET						
		RBRACKET = DIK_RBRACKET,	// �n�L�[			D_DIK_RBRACKET						
		AT = DIK_AT,	// ���L�[			D_DIK_AT							
		BACKSLASH = DIK_BACKSLASH,	// �_�L�[			D_DIK_BACKSLASH						
		COMMA = DIK_COMMA,	// �C�L�[			D_DIK_COMMA							
		KANJI = DIK_KANJI,	// �����L�[			D_DIK_KANJI							
		CONVERT = DIK_CONVERT,	// �ϊ��L�[			D_DIK_CONVERT						
		NOCONVERT = DIK_NOCONVERT,	// ���ϊ��L�[		D_DIK_NOCONVERT						
		KANA = DIK_KANA,	// �J�i�L�[			D_DIK_KANA							
		APPS = DIK_APPS,	// �A�v���P�[�V�������j���[�L�[		D_DIK_APPS			
		CAPSLOCK = DIK_CAPSLOCK,	// CaspLock�L�[		D_DIK_CAPSLOCK						
		SYSRQ = DIK_SYSRQ,	// PrintScreen�L�[	D_DIK_SYSRQ							
		PAUSE = DIK_PAUSE,	// PauseBreak�L�[	D_DIK_PAUSE							
		LWIN = DIK_LWIN,	// ��Win�L�[		D_DIK_LWIN							
		RWIN = DIK_RWIN,	// �EWin�L�[		D_DIK_RWIN							
		NUMLOCK = DIK_NUMLOCK,	// �e���L�[NumLock�L�[		D_DIK_NUMLOCK				
		NUMPAD0 = DIK_NUMPAD0,	// �e���L�[�O				D_DIK_NUMPAD0				
		NUMPAD1 = DIK_NUMPAD1,	// �e���L�[�P				D_DIK_NUMPAD1				
		NUMPAD2 = DIK_NUMPAD2,	// �e���L�[�Q				D_DIK_NUMPAD2				
		NUMPAD3 = DIK_NUMPAD3,	// �e���L�[�R				D_DIK_NUMPAD3				
		NUMPAD4 = DIK_NUMPAD4,	// �e���L�[�S				D_DIK_NUMPAD4				
		NUMPAD5 = DIK_NUMPAD5,	// �e���L�[�T				D_DIK_NUMPAD5				
		NUMPAD6 = DIK_NUMPAD6,	// �e���L�[�U				D_DIK_NUMPAD6				
		NUMPAD7 = DIK_NUMPAD7,	// �e���L�[�V				D_DIK_NUMPAD7				
		NUMPAD8 = DIK_NUMPAD8,	// �e���L�[�W				D_DIK_NUMPAD8				
		NUMPAD9 = DIK_NUMPAD9,	// �e���L�[�X				D_DIK_NUMPAD9				
		MULTIPLY = DIK_MULTIPLY,	// �e���L�[���L�[			D_DIK_MULTIPLY				
		ADD = DIK_ADD,	// �e���L�[�{�L�[			D_DIK_ADD					
		SUBTRACT = DIK_SUBTRACT,	// �e���L�[�|�L�[			D_DIK_SUBTRACT				
		DECIMAL = DIK_DECIMAL,	// �e���L�[�D�L�[			D_DIK_DECIMAL				
		DIVIDE = DIK_DIVIDE,	// �e���L�[�^�L�[			D_DIK_DIVIDE				
		NUMENTER = DIK_NUMPADENTER,	// �e���L�[�̃G���^�[�L�[	D_DIK_NUMPADENTER			
		F1 = DIK_F1,	// �e�P�L�[			D_DIK_F1							
		F2 = DIK_F2,	// �e�Q�L�[			D_DIK_F2							
		F3 = DIK_F3,	// �e�R�L�[			D_DIK_F3							
		F4 = DIK_F4,	// �e�S�L�[			D_DIK_F4							
		F5 = DIK_F5,	// �e�T�L�[			D_DIK_F5							
		F6 = DIK_F6,	// �e�U�L�[			D_DIK_F6							
		F7 = DIK_F7,	// �e�V�L�[			D_DIK_F7							
		F8 = DIK_F8,	// �e�W�L�[			D_DIK_F8							
		F9 = DIK_F9,	// �e�X�L�[			D_DIK_F9							
		F10 = DIK_F10,	// �e�P�O�L�[		D_DIK_F10							
		F11 = DIK_F11,	// �e�P�P�L�[		D_DIK_F11							
		F12 = DIK_F12,	// �e�P�Q�L�[		D_DIK_F12							
		A = DIK_A,	// �`�L�[			D_DIK_A								
		B = DIK_B,	// �a�L�[			D_DIK_B								
		C = DIK_C,	// �b�L�[			D_DIK_C								
		D = DIK_D,	// �c�L�[			D_DIK_D								
		E = DIK_E,	// �d�L�[			D_DIK_E								
		F = DIK_F,	// �e�L�[			D_DIK_F								
		G = DIK_G,	// �f�L�[			D_DIK_G								
		H = DIK_H,	// �g�L�[			D_DIK_H								
		I = DIK_I,	// �h�L�[			D_DIK_I								
		J = DIK_J,	// �i�L�[			D_DIK_J								
		K = DIK_K,	// �j�L�[			D_DIK_K								
		L = DIK_L,	// �k�L�[			D_DIK_L								
		M = DIK_M,	// �l�L�[			D_DIK_M								
		N = DIK_N,	// �m�L�[			D_DIK_N								
		O = DIK_O,	// �n�L�[			D_DIK_O								
		P = DIK_P,	// �o�L�[			D_DIK_P								
		Q = DIK_Q,	// �p�L�[			D_DIK_Q								
		R = DIK_R,	// �q�L�[			D_DIK_R								
		S = DIK_S,	// �r�L�[			D_DIK_S								
		T = DIK_T,	// �s�L�[			D_DIK_T								
		U = DIK_U,	// �t�L�[			D_DIK_U								
		V = DIK_V,	// �u�L�[			D_DIK_V								
		W = DIK_W,	// �v�L�[			D_DIK_W								
		X = DIK_X,	// �w�L�[			D_DIK_X								
		Y = DIK_Y,	// �x�L�[			D_DIK_Y								
		Z = DIK_Z,	// �y�L�[			D_DIK_Z								
		Key0 = DIK_0,	// �O�L�[			D_DIK_0								
		Key1 = DIK_1,	// �P�L�[			D_DIK_1								
		Key2 = DIK_2,	// �Q�L�[			D_DIK_2								
		Key3 = DIK_3,	// �R�L�[			D_DIK_3								
		Key4 = DIK_4,	// �S�L�[			D_DIK_4								
		Key5 = DIK_5,	// �T�L�[			D_DIK_5								
		Key6 = DIK_6,	// �U�L�[			D_DIK_6								
		Key7 = DIK_7,	// �V�L�[			D_DIK_7								
		Key8 = DIK_8,	// �W�L�[			D_DIK_8								
		Key9 = DIK_9,	// �X�L�[			D_DIK_9
	};


	enum class PADCODE
	{
		UP = XINPUT_GAMEPAD_DPAD_UP,
		DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LTHUMB = XINPUT_GAMEPAD_LEFT_THUMB,
		RTHUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
		LSHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RSHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		ABXY = A | B | X | Y,
		ALL = A | B | X | Y | UP | DOWN | LEFT | RIGHT | START | BACK | LTHUMB | RTHUMB | LSHOULDER | RSHOULDER,
	};
}
