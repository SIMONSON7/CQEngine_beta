#include "HelloWorld.h"

//
StartScene::StartScene()
{}

//
StartScene::~StartScene()
{}

//
StartScene*
StartScene::Create()
{
	STAGE_CREATE(StartScene);
}

//
void
StartScene::Destory(void* t)
{
	STAGE_DESTORY(StartScene);
}

//
void 
StartScene::Init()
{
	Actor2D* player = Actor2D::Create();
	Node* player_node0 = Node::Create();
	player_node0->Init(float3(32.5, 22.5, 15.f), "res/img/player.png");
	player_node0->SetScale(0.05f);
	player->AddNode(player_node0);

	Node* player_node1 = Node::Create();
	player_node1->Init(float3(32.5, 22.5, 5.f), "res/img/player.png");
	player_node1->SetScale(0.05f);
	player->AddNode(player_node1);

	this->AddActor2D(player);
}