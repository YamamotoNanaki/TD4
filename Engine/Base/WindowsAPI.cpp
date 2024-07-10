#include "WindowsAPI.h"
#include "GraphicsAPI.h"
//#include "Debug.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"winmm.lib")

using namespace IFE;

WindowsAPI* IFE::WindowsAPI::Instance()
{
	static WindowsAPI inst;
	return &inst;
}

void IFE::WindowsAPI::Finalize()
{
	timeEndPeriod(1);
	WindowsAPI::Instance()->Unregister();
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void IFE::WindowsAPI::Initialize(int32_t window_width, int32_t window_height, const LPCWSTR& windowName)
{
	timeBeginPeriod(1);
	wnd_.cbSize = sizeof(WNDCLASSEX);
	wnd_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	wnd_.lpszClassName = L"DirectX12Game"; // ウィンドウクラス名
	wnd_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	wnd_.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&wnd_);

	// 現在のカーソル位置を取得
	POINT pt;
	GetCursorPos(&pt);
	// 現在のモニターの情報を取得
	HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = {};
	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	winWidth_ = window_width;
	winHeight_ = window_height;

	//タイトルバー削除
	//AdjustWindowRect(&wrc, WS_POPUP, false); // 自動でサイズ補正

	//// ウィンドウオブジェクトの生成
	//hwnd = CreateWindow(w.lpszClassName, // クラス名
	//	windowName,			        // タイトルバーの文字
	//	WS_POPUP,        // 標準的なウィンドウスタイル
	//	CW_USEDEFAULT,              // 表示X座標（OSに任せる）
	//	CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
	//	wrc.right - wrc.left,       // ウィンドウ横幅
	//	wrc.bottom - wrc.top,   // ウィンドウ縦幅
	//	nullptr,                // 親ウィンドウハンドル
	//	nullptr,                // メニューハンドル
	//	w.hInstance,            // 呼び出しアプリケーションハンドル
	//	nullptr);               // オプション



	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(wnd_.lpszClassName, // クラス名
		windowName,			        // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
		mi.rcMonitor.left,
		mi.rcMonitor.top,
		wrc.right - wrc.left,       // ウィンドウ横幅
		wrc.bottom - wrc.top,   // ウィンドウ縦幅
		nullptr,                // 親ウィンドウハンドル
		nullptr,                // メニューハンドル
		wnd_.hInstance,            // 呼び出しアプリケーションハンドル
		nullptr);               // オプション

	// ウィンドウ表示
	ShowWindow(hwnd_, SW_SHOW);
	//#ifdef NDEBUG
	//	ShowCursor(false);
	//#endif
}

void IFE::WindowsAPI::Unregister()
{
	UnregisterClass(wnd_.lpszClassName, wnd_.hInstance);
}

void IFE::WindowsAPI::Update()
{
	RECT rect;
	GetClientRect(hwnd_, &rect);
	float width = static_cast<float>(rect.right - rect.left);
	float height = static_cast<float>(rect.bottom - rect.top);
	if (width == 0 && height == 0)return;
	if (winWidth_ == width && winHeight_ == height)
	{
		resize_ = false;
		return;
	}

	winWidth_ = int32_t(width);
	winHeight_ = int32_t(height);
	resize_ = true;
}

bool WindowsAPI::Message()
{
	// メッセージがある？
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_); // キー入力メッセージの処理
		DispatchMessage(&msg_);  //プロシージャにメッセージを送る
	}

	//Xボタンで終了メッセージが来たらゲームループを抜ける
	if (msg_.message == WM_QUIT)
	{
		return true;
	}
	return false;
}

HINSTANCE* IFE::WindowsAPI::GetHInstance()
{
	return &wnd_.hInstance;
}

HWND* IFE::WindowsAPI::GetHWnd()
{
	return &hwnd_;
}

LRESULT IFE::WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: //ウィンドーが破壊された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}
