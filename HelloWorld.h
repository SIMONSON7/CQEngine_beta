/*
*		user define class
*
*		qiu_hao    2016/5/1		6:33.pm
*/
#include "Scene.h"
#include "Node.h"

class StartScene:public Scene
{
private:
	StartScene();
	~StartScene();

public:
	static StartScene*	Create();
	static void	Destory(void* t);
	/*
	*	your game's Entities should be inited here ,then will be rended on screen;
	*
	*/
	void Init();

};

