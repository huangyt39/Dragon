#include "HardScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

cocos2d::Scene * HardScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//debug
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	scene->getPhysicsWorld()->setAutoStep(true);
	auto layer = HardScene::create();
	scene->addChild(layer);
	return scene;
}

bool HardScene::init() {
	if (!Layer::init()) {
		return false;
	}
}
