#pragma once
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include <cstdint>
#include "Float2.h"
#include "EditorMode.h"

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
		/// キーボード情報のアップデート
		/// </summary>
		/// <param name="result"></param>
		static void Update();

		//キーボードの判定
		/// <summary>
		/// トリガー判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool GetKeyTrigger(const Key& keyFlag);
		/// <summary>
		/// 入力判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool GetKeyPush(const Key& keyFlag);
		/// <summary>
		/// リリース判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool GetKeyRelease(const Key& keyFlag);
		//マウスの判定
		static bool GetMousePush(const Mouse& mouse);
		static bool GetMouseTrigger(const Mouse& mouse);
		static bool GetMouseRelease(const Mouse& mouse);
		static Float2 GetMousePoint();
		static int32_t GetMouseWheel();
		//コントローラーの判定
		static Float2 GetLAnalog(std::int32_t unresponsive_range = 1000);
		static Float2 GetRAnalog(std::int32_t unresponsive_range = 1000);
		static float GetLXAnalog(std::int32_t unresponsive_range = 1000);
		static float GetRXAnalog(std::int32_t unresponsive_range = 1000);
		static float GetLYAnalog(std::int32_t unresponsive_range = 1000);
		static float GetRYAnalog(std::int32_t unresponsive_range = 1000);
		static bool PadPush(const PADCODE& pad);
		static bool PadTrigger(const PADCODE& pad);
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
		BACK = DIK_BACK,	// BackSpaceキー	D_DIK_BACK							
		TAB = DIK_TAB,	// Tabキー			D_DIK_TAB							
		RETURN = DIK_RETURN,	// Enterキー		D_DIK_RETURN						
		Enter = DIK_RETURN,	// Enterキー		D_DIK_RETURN						
		LSHIFT = DIK_LSHIFT,	// 左Shiftキー		D_DIK_LSHIFT						
		RSHIFT = DIK_RSHIFT,	// 右Shiftキー		D_DIK_RSHIFT						
		LCONTROL = DIK_LCONTROL,	// 左Ctrlキー		D_DIK_LCONTROL						
		RCONTROL = DIK_RCONTROL,	// 右Ctrlキー		D_DIK_RCONTROL						
		Esc = DIK_ESCAPE,	// Escキー			D_DIK_ESCAPE						
		Space = DIK_SPACE,	// スペースキー		D_DIK_SPACE							
		PGUP = DIK_PGUP,	// PageUpキー		D_DIK_PGUP							
		PGDN = DIK_PGDN,	// PageDownキー		D_DIK_PGDN							
		END = DIK_END,	// Endキー			D_DIK_END							
		HOME = DIK_HOME,	// Homeキー			D_DIK_HOME							
		LEFT = DIK_LEFT,	// 左キー			D_DIK_LEFT							
		UP = DIK_UP,	// 上キー			D_DIK_UP							
		RIGHT = DIK_RIGHT,	// 右キー			D_DIK_RIGHT							
		DOWN = DIK_DOWN,	// 下キー			D_DIK_DOWN							
		INSERT = DIK_INSERT,	// Insertキー		D_DIK_INSERT						
		DELETE_Key = DIK_DELETE,	// Deleteキー		D_DIK_DELETE						
		MINUS = DIK_MINUS,	// －キー			D_DIK_MINUS							
		YEN = DIK_YEN,	// ￥キー			D_DIK_YEN							
		PREVTRACK = DIK_PREVTRACK,	// ＾キー			D_DIK_PREVTRACK						
		PERIOD = DIK_PERIOD,	// ．キー			D_DIK_PERIOD						
		SLASH = DIK_SLASH,	// ／キー			D_DIK_SLASH							
		LALT = DIK_LALT,	// 左Altキー		D_DIK_LALT							
		RALT = DIK_RALT,	// 右Altキー		D_DIK_RALT							
		SCROLL = DIK_SCROLL,	// ScrollLockキー	D_DIK_SCROLL						
		SEMICOLON = DIK_SEMICOLON,	// ；キー			D_DIK_SEMICOLON						
		COLON = DIK_COLON,	// ：キー			D_DIK_COLON							
		LBRACKET = DIK_LBRACKET,	// ［キー			D_DIK_LBRACKET						
		RBRACKET = DIK_RBRACKET,	// ］キー			D_DIK_RBRACKET						
		AT = DIK_AT,	// ＠キー			D_DIK_AT							
		BACKSLASH = DIK_BACKSLASH,	// ＼キー			D_DIK_BACKSLASH						
		COMMA = DIK_COMMA,	// ，キー			D_DIK_COMMA							
		KANJI = DIK_KANJI,	// 漢字キー			D_DIK_KANJI							
		CONVERT = DIK_CONVERT,	// 変換キー			D_DIK_CONVERT						
		NOCONVERT = DIK_NOCONVERT,	// 無変換キー		D_DIK_NOCONVERT						
		KANA = DIK_KANA,	// カナキー			D_DIK_KANA							
		APPS = DIK_APPS,	// アプリケーションメニューキー		D_DIK_APPS			
		CAPSLOCK = DIK_CAPSLOCK,	// CaspLockキー		D_DIK_CAPSLOCK						
		SYSRQ = DIK_SYSRQ,	// PrintScreenキー	D_DIK_SYSRQ							
		PAUSE = DIK_PAUSE,	// PauseBreakキー	D_DIK_PAUSE							
		LWIN = DIK_LWIN,	// 左Winキー		D_DIK_LWIN							
		RWIN = DIK_RWIN,	// 右Winキー		D_DIK_RWIN							
		NUMLOCK = DIK_NUMLOCK,	// テンキーNumLockキー		D_DIK_NUMLOCK				
		NUMPAD0 = DIK_NUMPAD0,	// テンキー０				D_DIK_NUMPAD0				
		NUMPAD1 = DIK_NUMPAD1,	// テンキー１				D_DIK_NUMPAD1				
		NUMPAD2 = DIK_NUMPAD2,	// テンキー２				D_DIK_NUMPAD2				
		NUMPAD3 = DIK_NUMPAD3,	// テンキー３				D_DIK_NUMPAD3				
		NUMPAD4 = DIK_NUMPAD4,	// テンキー４				D_DIK_NUMPAD4				
		NUMPAD5 = DIK_NUMPAD5,	// テンキー５				D_DIK_NUMPAD5				
		NUMPAD6 = DIK_NUMPAD6,	// テンキー６				D_DIK_NUMPAD6				
		NUMPAD7 = DIK_NUMPAD7,	// テンキー７				D_DIK_NUMPAD7				
		NUMPAD8 = DIK_NUMPAD8,	// テンキー８				D_DIK_NUMPAD8				
		NUMPAD9 = DIK_NUMPAD9,	// テンキー９				D_DIK_NUMPAD9				
		MULTIPLY = DIK_MULTIPLY,	// テンキー＊キー			D_DIK_MULTIPLY				
		ADD = DIK_ADD,	// テンキー＋キー			D_DIK_ADD					
		SUBTRACT = DIK_SUBTRACT,	// テンキー－キー			D_DIK_SUBTRACT				
		DECIMAL = DIK_DECIMAL,	// テンキー．キー			D_DIK_DECIMAL				
		DIVIDE = DIK_DIVIDE,	// テンキー／キー			D_DIK_DIVIDE				
		NUMENTER = DIK_NUMPADENTER,	// テンキーのエンターキー	D_DIK_NUMPADENTER			
		F1 = DIK_F1,	// Ｆ１キー			D_DIK_F1							
		F2 = DIK_F2,	// Ｆ２キー			D_DIK_F2							
		F3 = DIK_F3,	// Ｆ３キー			D_DIK_F3							
		F4 = DIK_F4,	// Ｆ４キー			D_DIK_F4							
		F5 = DIK_F5,	// Ｆ５キー			D_DIK_F5							
		F6 = DIK_F6,	// Ｆ６キー			D_DIK_F6							
		F7 = DIK_F7,	// Ｆ７キー			D_DIK_F7							
		F8 = DIK_F8,	// Ｆ８キー			D_DIK_F8							
		F9 = DIK_F9,	// Ｆ９キー			D_DIK_F9							
		F10 = DIK_F10,	// Ｆ１０キー		D_DIK_F10							
		F11 = DIK_F11,	// Ｆ１１キー		D_DIK_F11							
		F12 = DIK_F12,	// Ｆ１２キー		D_DIK_F12							
		A = DIK_A,	// Ａキー			D_DIK_A								
		B = DIK_B,	// Ｂキー			D_DIK_B								
		C = DIK_C,	// Ｃキー			D_DIK_C								
		D = DIK_D,	// Ｄキー			D_DIK_D								
		E = DIK_E,	// Ｅキー			D_DIK_E								
		F = DIK_F,	// Ｆキー			D_DIK_F								
		G = DIK_G,	// Ｇキー			D_DIK_G								
		H = DIK_H,	// Ｈキー			D_DIK_H								
		I = DIK_I,	// Ｉキー			D_DIK_I								
		J = DIK_J,	// Ｊキー			D_DIK_J								
		K = DIK_K,	// Ｋキー			D_DIK_K								
		L = DIK_L,	// Ｌキー			D_DIK_L								
		M = DIK_M,	// Ｍキー			D_DIK_M								
		N = DIK_N,	// Ｎキー			D_DIK_N								
		O = DIK_O,	// Ｏキー			D_DIK_O								
		P = DIK_P,	// Ｐキー			D_DIK_P								
		Q = DIK_Q,	// Ｑキー			D_DIK_Q								
		R = DIK_R,	// Ｒキー			D_DIK_R								
		S = DIK_S,	// Ｓキー			D_DIK_S								
		T = DIK_T,	// Ｔキー			D_DIK_T								
		U = DIK_U,	// Ｕキー			D_DIK_U								
		V = DIK_V,	// Ｖキー			D_DIK_V								
		W = DIK_W,	// Ｗキー			D_DIK_W								
		X = DIK_X,	// Ｘキー			D_DIK_X								
		Y = DIK_Y,	// Ｙキー			D_DIK_Y								
		Z = DIK_Z,	// Ｚキー			D_DIK_Z								
		Key0 = DIK_0,	// ０キー			D_DIK_0								
		Key1 = DIK_1,	// １キー			D_DIK_1								
		Key2 = DIK_2,	// ２キー			D_DIK_2								
		Key3 = DIK_3,	// ３キー			D_DIK_3								
		Key4 = DIK_4,	// ４キー			D_DIK_4								
		Key5 = DIK_5,	// ５キー			D_DIK_5								
		Key6 = DIK_6,	// ６キー			D_DIK_6								
		Key7 = DIK_7,	// ７キー			D_DIK_7								
		Key8 = DIK_8,	// ８キー			D_DIK_8								
		Key9 = DIK_9,	// ９キー			D_DIK_9
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
