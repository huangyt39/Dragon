#include "HardScene.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "string.h"

#define database UserDefault::getInstance()

using namespace cocos2d::network;

USING_NS_CC;

cocos2d::Scene * HardScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//debug
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

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
	auto origin = Director::getInstance()->getVisibleOrigin();

	// 添加背景
	auto bg = Sprite::create("background.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	//添加龙
	dragon = new Dragon(DRAGONBLUE, Vec2(100, visibleSize.height / 2));
	this->addChild(dragon->get());
	//添加柱子
	barrier = new Barrier(4, 2, this);
	
	// 获取历史最高分数
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}
	int best = database->getIntegerForKey("hardbest");

	// 添加分数
	auto yBest = Label::createWithTTF("Best Score: ", "fonts/arial.ttf", 28);
	yBest->setPosition(Vec2(origin.x + visibleSize.width - 120, origin.y + visibleSize.height - 30));
	//this->addChild(yBest, 4);

	Best = Label::createWithTTF(Value(best).asString(), "fonts/arial.ttf", 28);
	Best->setPosition(Vec2(origin.x + visibleSize.width - 30, origin.y + visibleSize.height - 30));
	//this->addChild(Best, 4);

	auto yScore = Label::createWithTTF("Your Score: ", "fonts/arial.ttf", 28);
	yScore->setPosition(Vec2(origin.x + visibleSize.width - 120, origin.y + visibleSize.height - 60));
	//this->addChild(yScore, 4);

	Score = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	Score->setPosition(Vec2(origin.x + visibleSize.width - 30, origin.y + visibleSize.height - 60));
	//this->addChild(Score, 4);

	// 添加监听器
	addListener();

	// 调度器
	schedule(schedule_selector(HardScene::checkAll), 0.04f, kRepeatForever, 0);

	//游戏开始
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
	// 添加键盘事件监听器
	auto Keyboardlistener = EventListenerKeyboard::create();
	Keyboardlistener->onKeyPressed = CC_CALLBACK_2(HardScene::onKeyPressed, this);
	Keyboardlistener->onKeyReleased = CC_CALLBACK_2(HardScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Keyboardlistener, this);
	// 添加碰撞监听器
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HardScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HardScene::checkAll(float f) {
	// check if the dragon pass a barrier, if so, get one point
	if (barrier->check(dragon->get()->getPosition())) {
		score++;
		char str[10];
		sprintf(str, "%d", score);
		Score->setString(str);
	}
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

	gameover();
	uploadScore();

	return true;
}

void HardScene::gameover() {
	// gameover提示
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* gameover = Sprite::create("gameover.png");
	gameover->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(gameover, 3);

	auto restart = MenuItemFont::create("Restart", [](Ref *pSender) {
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8, HardScene::createScene
		()));
	});
	if (restart) {
		restart->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - restart->getContentSize().height / 2 - 100));
	}

	auto back = MenuItemFont::create("Back", [](Ref *pSender) {
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8, MenuScene::createScene
		()));
	});
	if (back) {
		back->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - restart->getContentSize().height / 2 - back->getContentSize().height - 120));
	}
	auto menu = Menu::create(restart, back, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void HardScene::uploadScore() {
	// 记录分数
	if (database->getIntegerForKey("hardbest") < score) {
		database->setIntegerForKey("hardbest", score);
	}
}