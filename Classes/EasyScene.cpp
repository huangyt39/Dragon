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

bool EasyScene::init(){
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//Ìí¼ÓÁú
	dragon = new Dragon(DRAGONBLUE, Vec2(100, visibleSize.height/2));
	this->addChild(dragon->get());
	//Ìí¼ÓÖù×Ó
	barrier = new Barrier(4, 0, this);
	
	// Ìí¼Ó¼àÌýÆ÷
	addListener();
	
	// µ÷¶ÈÆ÷
	schedule(schedule_selector(EasyScene::checkAll), 0.04f, kRepeatForever, 0);

	//ÓÎÏ·¿ªÊ¼
	dragon->begin(true);
	barrier->begin();
	over = false;

	return true;
}


void EasyScene::addListener() {
	// Ìí¼Ó´¥ÃþÊÂ¼þ¼àÌýÆ÷
	EventListenerTouchOneByOne* Touchlistener = EventListenerTouchOneByOne::create();
	Touchlistener->setSwallowTouches(true);
	Touchlistener->onTouchBegan = CC_CALLBACK_2(EasyScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Touchlistener, this);
	// Ìí¼ÓÅö×²¼àÌýÆ÷
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(EasyScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool EasyScene::onTouchBegan(Touch *touch, Event *event) {
	if (!over) {
		dragon->jump();
	}
	return true;
}

void EasyScene::checkAll(float f) {
	barrier->check(dragon->get()->getPosition());
	dragon->check();
}

bool EasyScene::onConcactBegin(PhysicsContact & contact) {
	over = true;
	barrier->end();
	dragon->die();
	_eventDispatcher->removeAllEventListeners();
	unschedule(schedule_selector(EasyScene::checkAll));
	delete barrier;
	delete dragon;
	return true;
}