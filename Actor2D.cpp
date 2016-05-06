#include "Node.h"
#include "Actor2D.h"
#include "Stage.h"

//
Actor2D::Actor2D() :m_nodeArray(nullptr)
{

}

//
Actor2D::~Actor2D()
{

}

//
Actor2D*	
Actor2D::Create()
{
	STAGE_CREATE(Actor2D);
}

//
void 
Actor2D::Destory(void* t)
{
	STAGE_DESTORY(Actor2D);
}

//
void
Actor2D::AddNode(Node* _node)
{
	if (!m_nodeArray)
	{
		m_nodeArray = Array<Node*>::Create();
	}
	if (_node)
	{
		m_nodeArray->Push(_node);
	}
}

//
void
Actor2D::Show()
{
	if (!m_nodeArray)
	{
		return;
	}

	int i = 0;
	int len = m_nodeArray->GetLastIndex();
	Node* node;
	while (i < len)
	{
		node = (*m_nodeArray)[i];
		node->Show();
		i++;
	}
}