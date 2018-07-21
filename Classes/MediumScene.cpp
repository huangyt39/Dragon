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

	auto visibleSize = Director::getInstance()->getVisibleSize();

	//Ìí¼ÓÁú
	dragon = new Dragon(DRAGONBLUE, Vec2(100, visibleSize.height / 2));
	this->addChild(dragon->get());
	//Ìí¼ÓÖù×Ó
	barrier = new Barrier(4, 1, this);

	// Ìí¼Ó¼àÌýÆ÷
	addListener();

	// µ÷¶ÈÆ÷
	schedule(schedule_selector(MediumScene::checkAll), 0.04f, kRepeatForever, 0);

	//ÓÎÏ·¿ªÊ¼
	dragon->begin(false);
	barrier->begin();

	return true;
}

void MediumScene::addListener() {
	// Ìí¼Ó¼üÅÌÊÂ¼þ¼àÌýÆ÷
	auto Keyboardlistener = EventListenerKeyboard::create();
	Keyboardlistener->onKeyPressed = CC_CALLBACK_2(MediumScene::onKeyPressed, this);
	Keyboardlistener->onKeyReleased = CC_CALLBACK_2(MediumScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Keyboardlistener, this);
	// Ìí¼ÓÅö×²¼àÌýÆ÷
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(MediumScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void MediumScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		movekey = 'W';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		movekey = 'S';
		isMove = true;
		break;
	default:
		break;
	}
}

void MediumScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
		if (movekey == 'W') {
			dragon->stop();
			isMove = false;
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
		if (movekey == 'S') {
			dragon->stop();
			isMove = false;
		}
		break;
	default:
		break;
	}
}


void MediumScene::checkAll(float f) {
	barrier->check(dragon->get()->getPosition());
	dragon->check();

	if (isMove) {
		this->move(movekey);
	}
}

void MediumScene::move(char c) {
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

bool MediumScene::onConcactBegin(PhysicsContact & contact) {
	barrier->end();
	dragon->die();
	_eventDispatcher->removeAllEventListeners();
	unschedule(schedule_selector(MediumScene::checkAll));
	delete barrier;
	delete dragon;
	return true;
}
