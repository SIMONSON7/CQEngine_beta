#include "Scene.h"

//
Scene::Scene() :m_actorArray(nullptr)
{

}

//
Scene::~Scene()
{
	// no need to destory the Array, beacause the pool will to do
}

/*
 *	- init every Actor2D/3D here
 *
 */
void
Scene::Init()
{
	//
}

//
void
Scene::AddActor2D(Actor2D* _actor) // tag
{
	if (!m_actorArray)
	{
		m_actorArray = Array<Actor2D*>::Create();
	}
	if (_actor)
	{
		m_actorArray->Push(_actor); // Push(tag,_actor)
	}
}

/*
void 
Scene::RemoveActor2D(Actor2D* _actor)
{
	if (m_actorArray && _actor)
	{
		m_actorArray->Pop();
	}
}
*/

//
void
Scene::Show()
{
	if (!m_actorArray)
	{
		return;
	}

	int i = 0;
	int len = m_actorArray->GetLastIndex();
	Actor2D* actor;
	while (i < len)
	{
		actor = (*m_actorArray)[i];
		actor->Show();
		i++;
	}
}







