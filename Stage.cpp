#include "Stage.h"
#include "Scene.h"
#include "Node.h"
#include "TimeStamp.h"


static Stage g_stage;

Stage::Stage()
	:
	m_nAnimationInterval(1 / 60.f), m_startScene(nullptr),
	mm(nullptr), shader(nullptr), defaultCamera(nullptr)
{

}

Stage::~Stage()
{
	if (mm)
	{
		delete mm;
	}
	if (shader)
	{
		delete shader;
	}
	if (defaultCamera)
	{
		delete defaultCamera;
	}
}

//
Stage*
Stage::AccessStage()
{
	static bool s_bFirstUseStage = true;
	if (s_bFirstUseStage)
	{
		s_bFirstUseStage = false;
		g_stage.__initEnvironment();
	}
	return &g_stage;
}

/*
 *	- create a win32 window
 *	- __initEGL()
 *
 */
void
Stage::BuildStage(const HINSTANCE _hinstance, TCHAR* _appName, const size_t _winWidth, const size_t _winHeight, const double _timeInterval)
{
	m_nAnimationInterval = _timeInterval;

	win.Init(_hinstance, _appName, _winWidth, _winHeight);
	win.RegisterWnd();
	win.CreateWnd();
	win.DisplayWnd();

	// 
	__initEGL(); 
}

//
void
Stage::SetStartScene(Scene* _startScene)
{
	m_startScene = _startScene;
}


/*
*	- init OpenGL ES
*	- init default shaderProgram obj
*	- init default 3rd camera
*/
EGLConfig			config;
EGLSurface 			surface;
EGLContext 			context;
EGLDisplay			display;
bool
Stage::__initEGL()
{
	//
	const EGLint attribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};
	EGLint 	format;
	EGLint	numConfigs;
	EGLint  major;
	EGLint  minor;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, &major, &minor);

	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	surface = eglCreateWindowSurface(display, config, win.m_hWnd, NULL);

	EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	context = eglCreateContext(display, config, 0, attr);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		return false;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &win.m_winWidth);
	eglQuerySurface(display, surface, EGL_HEIGHT, &win.m_winHeight);

	SendMessage(win.m_hWnd, WM_SIZE, 0, 0);

	// default shader
	const char* vs_file = "shader/default_VS.vs";
	const char* fs_file = "shader/default_FS.fs";
	shader = new Shader(vs_file, fs_file);

	// default camera
	defaultCamera = new Camera3rd();
	defaultCamera->Init(50, float3(50, 50, 50), float3(20, 10, -5), float3(0, 1, 0));

	return true;
}

/*
*
*	- glViewport()
*	- run with the 1st Scene
*
*/
bool
Stage::__isStageReady()
{
	//
	glViewport(0, 0, win.m_winWidth, win.m_winHeight);

	//
	if (!m_startScene)
	{
		return false;
	}
	return true;
}

/*
*
*/
void
Stage::__drawScene()
{
	// shader
	shader->UseShader();

	// 
	if (!m_startScene) return;

	// clear color buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update camera
	defaultCamera->Update();

	// render
	{
		m_startScene->Show();
	}

	// swap buffer
	eglSwapBuffers(display, surface);
}


/*
*
*	- init memory manager
*/
void
Stage::__initEnvironment()
{
	// memory manager
	mm = new CQ::MemManager();
}

/*
 *	- __endEGL()
 *	- unuse default shader
 */
void
Stage::__finishEnvironment()
{
	__endEGL();

	shader->UnuseShader();
}

/*
*	- main loop
*/
void
Stage::Show()
{
	MSG msg = { 0 };

	Timestamp time;
	time.record();

	if (!__isStageReady())
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);

	while (msg.message != WM_QUIT)
	{
		if (msg.message == WM_DESTROY || msg.message == WM_CLOSE)
		{
			break;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			double stamp = time.getElapsedSecond();
			if (stamp > m_nAnimationInterval)
			{
				time.record();
				// mainLoop :render
				__drawScene();
			}
			else
			{
				Sleep(0);
			}
		}
	}
	__finishEnvironment();
}

/*
*	- finish openGLES
*/
void
Stage::__endEGL()
{
	if (display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;

	UnregisterClass(win.m_appName, win.m_hInstance);
}


