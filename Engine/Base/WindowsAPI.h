#pragma once
#include <Windows.h>
#include <cstdint>

namespace IFE
{
	class WindowsAPI final
	{
	private:
		WNDCLASSEX wnd_{}; // ウィンドウクラスの設定
		HWND hwnd_{};
		MSG msg_{}; // メッセージ
		WindowsAPI() {}
		WindowsAPI(const WindowsAPI&) {}
		WindowsAPI& operator=(const WindowsAPI&) {}
		~WindowsAPI() {}
	public:
		int32_t winWidth_ = 0;
		int32_t winHeight_ = 0;
		bool resize_ = false;

	public:
		static WindowsAPI* Instance();
		static void Finalize();
		void Initialize(int32_t window_width, int32_t window_height, const LPCWSTR& windowName = L"DirectXGame");
		void Unregister();
		void Update();
		bool Message();
		HINSTANCE* GetHInstance();
		HWND* GetHWnd();

		//ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}