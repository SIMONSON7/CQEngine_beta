/*
*
*		qiu_hao		2016/5/3		10:21.pm
*/
#ifndef __ACTOR2D_H__
#define __ACTOR2D_H__

extern class Node;

class Actor2D
{
public:

private:
	Actor2D();
	~Actor2D();
	
public:
	static Actor2D*	Create();
	static void Destory(void* _t);
	void AddNode(Node* _node);
	void Show();

public:
	Array<Node*>* m_nodeArray;
};

#endif	/* __ACTOR2D_H__ */

































