#include "MediumScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

cocos2d::Scene * MediumScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//debug
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	scene->getPhysicsWorld()->setAutoStep(true);
	auto layer = MediumScene::create();
	scene->addChild(layer);
	return scene;
}

bool MediumScene::init() {
	if (!Layer::init()) {
		return false;

	}
}
