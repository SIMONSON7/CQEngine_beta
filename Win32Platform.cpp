#include "Win32Platform.h"

LRESULT CALLBACK Win32Platform::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CREATE:
	{
		
	}
		break;
	case WM_COMMAND:
	{
		
	}
		break;
	case WM_PAINT:
		break;
	case WM_LBUTTONDOWN:
	{
		
	}
		break;
	case WM_LBUTTONUP:
	{
		
	}
		break;
	case WM_RBUTTONDOWN://ÓÒ¼üµ¯³ö¡°ÓÒ¼ü²Ëµ¥¡±
	{
		
	}
		break;
	case WM_RBUTTONUP:
	{
		
	}
		break;
	case WM_MOUSEMOVE:
	{

	}
		break;
	case WM_LBUTTONDBLCLK:
		
		break;
	case WM_MOUSEWHEEL:
	{
		
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, nMsg,
		wParam, lParam);
}

void Win32Platform::Init( const HINSTANCE _hinstance,   TCHAR* _appName, const int _winWidth, const int _winHeight)
{
	m_hInstance = _hinstance;
	m_hWnd = 0;
	m_appName = _appName;
	m_winWidth = _winWidth;
	m_winHeight = _winHeight;
}

void Win32Platform::RegisterWnd()
{
	WNDCLASSEX wce = { 0 };
	wce.cbSize = sizeof(wce);
	wce.cbClsExtra = 0;
	wce.cbWndExtra = 0;
	wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wce.hCursor = NULL;
	wce.hIcon = LoadCursor(m_hInstance, IDI_APPLICATION);
	wce.hIconSm = NULL;
	wce.hInstance = m_hInstance;
	wce.lpfnWndProc = WndProc;
	wce.lpszClassName = m_appName;
	wce.lpszMenuName = NULL;
	wce.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	ATOM nAtom = RegisterClassEx(&wce);
	if (0 == nAtom)
	{
		return ;
	}
}

void Win32Platform::CreateWnd()
{
	m_hWnd = CreateWindowEx(0,
					m_appName, 
					m_appName,
					WS_OVERLAPPEDWINDOW  & ~WS_MAXIMIZEBOX ^ WS_THICKFRAME,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					m_winWidth,
					m_winHeight, NULL, NULL,
					m_hInstance, NULL);
}

void Win32Platform::DisplayWnd()
{
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

