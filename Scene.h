/*
*		
*
*		qiu_hao    2016/5/3		5:33.pm
*/
#ifndef __SCENE_H__
#define __SCENE_H__


#include "Array.h"
#include "Actor2D.h"

class Scene // : public Entity
{
public:
	Scene();
	~Scene();

public:
	virtual void Init();
	virtual void AddActor2D(Actor2D* _actor);
	//virtual void RemoveActor2D(Actor2D* _actor);
	virtual void Show();

public:
	Array<Actor2D*>* m_actorArray;

};


#endif /* _SCENE_H__ */