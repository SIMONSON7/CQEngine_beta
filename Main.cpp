/*
*
*		qiu_hao    2016/5/1		6:26.pm
*/
#include "Main.h"
#include "Stage.h"
#include "HelloWorld.h"


#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInatance,
	LPSTR lpCmdLine,
	int nCmdSho)
{
	Stage* s = Stage::AccessStage();
	s->BuildStage(hInstance,__T("HelloWorld"), 800, 600,1/60.f);
	StartScene* scene = StartScene::Create();
	scene->Init();
	s->SetStartScene(scene);
	s->Show();

	//------------------------------------------------------------
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}






