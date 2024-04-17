#include "WindowsAPI.h"
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
	wnd_.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
	wnd_.lpszClassName = L"DirectX12Game"; // �E�B���h�E�N���X��
	wnd_.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	wnd_.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&wnd_);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, window_width, window_height };
	winWidth_ = window_width;
	winHeight_ = window_height;

	//�^�C�g���o�[�폜
	//AdjustWindowRect(&wrc, WS_POPUP, false); // �����ŃT�C�Y�␳

	//// �E�B���h�E�I�u�W�F�N�g�̐���
	//hwnd = CreateWindow(w.lpszClassName, // �N���X��
	//	windowName,			        // �^�C�g���o�[�̕���
	//	WS_POPUP,        // �W���I�ȃE�B���h�E�X�^�C��
	//	CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
	//	CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
	//	wrc.right - wrc.left,       // �E�B���h�E����
	//	wrc.bottom - wrc.top,   // �E�B���h�E�c��
	//	nullptr,                // �e�E�B���h�E�n���h��
	//	nullptr,                // ���j���[�n���h��
	//	w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
	//	nullptr);               // �I�v�V����



	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(wnd_.lpszClassName, // �N���X��
		windowName,			        // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,       // �E�B���h�E����
		wrc.bottom - wrc.top,   // �E�B���h�E�c��
		nullptr,                // �e�E�B���h�E�n���h��
		nullptr,                // ���j���[�n���h��
		wnd_.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
		nullptr);               // �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(hwnd_, SW_SHOW);
#ifdef NDEBUG
	ShowCursor(false);
#endif
}

void IFE::WindowsAPI::Unregister()
{
	UnregisterClass(wnd_.lpszClassName, wnd_.hInstance);
}

bool WindowsAPI::Message()
{
	// ���b�Z�[�W������H
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg_);  //�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//X�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
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

	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: //�E�B���h�[���j�󂳂ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}
