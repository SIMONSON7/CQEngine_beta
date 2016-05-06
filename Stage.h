/*
*		- 初始化 EGL 环境；
*		- 设置默认帧率；
*
*
*
*		qiu_hao    2016/5/1		5:17.pm
*/
#ifndef __STAGE_H__
#define __STAGE_H__

#include "Win32Platform.h"
#include "Allocator.h"
#include "Shader.h"
#include "Camera3rd.h"
#include <new>
#include <EGL/egl.h>
#include <gles2/gl2.h>
#include "freeImage/FreeImage.h"

extern class Scene;

class Stage
{
public:
	// singleton
	static Stage* AccessStage();
	void BuildStage(const HINSTANCE,  TCHAR* _appName, const size_t _winWidth, const size_t _winHeight,const double _timeInterval);
	void SetStartScene(Scene* _startScene);
	void Show();

public:
	Stage();
	~Stage();

private:
	bool __initEGL();
	bool __isStageReady();
	void __drawScene();
	void __endEGL();
	void __initEnvironment();
	void __finishEnvironment();

public:
	Win32Platform					win;
	double							m_nAnimationInterval;

	CQ::MemManager*					mm ;
	Shader*							shader;
	Camera3rd*						defaultCamera;

	Scene*							m_startScene;

};

/******************* CONVENIENT MACRO ***********************/
//
#define STAGE_CREATE(TYPE)							\
		Stage* s = Stage::AccessStage();				\
		void* buffer = s->mm->allocate(sizeof(TYPE),Destory);	\
		return new(buffer)TYPE()

//
#define STAGE_DESTORY(TYPE)		static_cast<TYPE*>(t)->~TYPE()


#endif

