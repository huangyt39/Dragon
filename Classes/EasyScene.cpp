#include "EasyScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

cocos2d::Scene * EasyScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//debug
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	scene->getPhysicsWorld()->setAutoStep(true);
	auto layer = EasyScene::create();
	scene->addChild(layer);
	return scene;
}

bool EasyScene::init() {
	if (!Layer::init()) {
		return false;
	}
	Dragon dragon(DRAGONBLUE, Vec2(100, 100));
	this->addChild(dragon.get());
	return true;
}
