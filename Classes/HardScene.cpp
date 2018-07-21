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
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//Ìí¼ÓÁú
	dragon = new Dragon(DRAGONBLUE, Vec2(100, visibleSize.height / 2));
	this->addChild(dragon->get());
	//Ìí¼ÓÖù×Ó
	barrier = new Barrier(4, 2, this);

	// Ìí¼Ó¼àÌýÆ÷
	addListener();

	// µ÷¶ÈÆ÷
	schedule(schedule_selector(HardScene::checkAll), 0.04f, kRepeatForever, 0);

	//ÓÎÏ·¿ªÊ¼
	dragon->begin(false);
	barrier->begin();

	return true;
}

void HardScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		Bmovekey = 'U';
		BMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		Dmovekey = 'W';
		DMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		Bmovekey = 'D';
		BMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		Dmovekey = 'S';
		DMove = true;
		break;
	default:
		break;
	}
}

void HardScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (Bmovekey == 'U') {
			barrier->stop();
			BMove = false;
		}
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (Bmovekey == 'D') {
			barrier->stop();
			BMove = false;
		}
		break;
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
		if (Dmovekey == 'W') {
			dragon->stop();
			DMove = false;
		}
		break;
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
		if (Dmovekey == 'S') {
			dragon->stop();
			DMove = false;
		}
		break;
	default:
		break;
	}
}

void HardScene::addListener() {
	// Ìí¼Ó¼üÅÌÊÂ¼þ¼àÌýÆ÷
	auto Keyboardlistener = EventListenerKeyboard::create();
	Keyboardlistener->onKeyPressed = CC_CALLBACK_2(HardScene::onKeyPressed, this);
	Keyboardlistener->onKeyReleased = CC_CALLBACK_2(HardScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Keyboardlistener, this);
	// Ìí¼ÓÅö×²¼àÌýÆ÷
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HardScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HardScene::checkAll(float f) {
	barrier->check(dragon->get()->getPosition());
	dragon->check();

	if (BMove) {
		this->barrierMove(Bmovekey);
	}
	if (DMove) {
		this->dargonMove(Dmovekey);
	}
}

void HardScene::dargonMove(char c) {
	switch (c) {
	case 'W':
		dragon->move(1);
		break;
	case 'S':
		dragon->move(-1);
		break;
	default:
		break;
	}
}

void HardScene::barrierMove(char c) {
	switch (c) {
	case 'U':
		barrier->move(dragon->get()->getPosition(), 1);
		break;
	case 'D':
		barrier->move(dragon->get()->getPosition(), -1);
		break;
	default:
		break;
	}
}

bool HardScene::onConcactBegin(PhysicsContact & contact) {
	barrier->end();
	dragon->die();
	_eventDispatcher->removeAllEventListeners();
	unschedule(schedule_selector(HardScene::checkAll));
	delete barrier;
	delete dragon;
	return true;
}