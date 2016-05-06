/*
*		临时文件
*		- 创建 win32 窗口 ;
*		- 事件处理（tmp）;
*
*
*
*		qiu_hao    2016/5/1		8:25.
*/
#ifndef __WIN32PLATFORM_H__
#define __WIN32PLATFORM_H__
#include <windows.h>

class Win32Platform
{
public:
	Win32Platform(){}
	~Win32Platform(){}
	void Init(const HINSTANCE _hinstance,  TCHAR* _appName, const int _winWidth, const int _winHeight);
	void RegisterWnd();
	void CreateWnd();
	void DisplayWnd();

	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
public:
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;
	TCHAR*	  m_appName;

	int		  m_winWidth;
	int		  m_winHeight;
};

#endif