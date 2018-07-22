#include "EasyScene.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "string.h"
#include "ui\CocosGUI.h"

#define database UserDefault::getInstance()

using namespace cocos2d::network;
using namespace cocos2d::ui;

USING_NS_CC;

cocos2d::Scene * EasyScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//debug
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	scene->getPhysicsWorld()->setAutoStep(true);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
	auto layer = EasyScene::create();
	scene->addChild(layer);
	return scene;
}

bool EasyScene::init(){
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// ��ӱ���
	auto bg = Sprite::create("background.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	//�����
	dragon = new Dragon(DRAGONBLUE, Vec2(100, visibleSize.height/2));
	this->addChild(dragon->get());
	//�������
	barrier = new Barrier(4, 0, this);

	// ��ȡ��ʷ��߷���
	if (!database->getBoolForKey("isExist")) {
		database->setBoolForKey("isExist", true);
	}
	int best = database->getIntegerForKey("easybest");

	// ��ӷ���
	auto yBest = Label::createWithTTF("Best Score: ", "fonts/arial.ttf", 28);
	yBest->setPosition(Vec2(origin.x + visibleSize.width - 120, origin.y + visibleSize.height - 30));
	this->addChild(yBest, 4);
	Best = Label::createWithTTF(Value(best).asString(), "fonts/arial.ttf", 28);
	Best->setPosition(Vec2(origin.x + visibleSize.width - 30, origin.y + visibleSize.height - 30));
	this->addChild(Best, 4);

	auto yScore = Label::createWithTTF("Your Score: ", "fonts/arial.ttf", 28);
	yScore->setPosition(Vec2(origin.x + visibleSize.width - 120, origin.y + visibleSize.height - 60));
	this->addChild(yScore, 4);

	Score = Label::createWithTTF("0", "fonts/arial.ttf", 28);
	Score->setPosition(Vec2(origin.x + visibleSize.width - 30, origin.y + visibleSize.height - 60));
	this->addChild(Score, 4);
	
	// ��Ӽ�����
	addListener();
	
	// ������
	schedule(schedule_selector(EasyScene::checkAll), 0.04f, kRepeatForever, 0);

	//��Ϸ��ʼ
	dragon->begin(true);
	barrier->begin();
	over = false;

	return true;
}

void EasyScene::addListener() {
	// ��Ӵ����¼�������
	EventListenerTouchOneByOne* Touchlistener = EventListenerTouchOneByOne::create();
	Touchlistener->setSwallowTouches(true);
	Touchlistener->onTouchBegan = CC_CALLBACK_2(EasyScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Touchlistener, this);
	// �����ײ������
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(EasyScene::onConcactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool EasyScene::onTouchBegan(Touch *touch, Event *event) {
	if (!over) {
		dragon->jump();
		this->scheduleOnce(schedule_selector(EasyScene::down), 0.7f);
	}
	return true;
}

void EasyScene::checkAll(float f) {
	// check if the dragon pass a barrier, if so, get one point
	if (barrier->check(dragon->get()->getPosition())) {
		score++;
		char str[10];
		sprintf(str, "%d", score);
		Score->setString(str);
	}
	dragon->check();
}

bool EasyScene::onConcactBegin(PhysicsContact & contact) {
	over = true;
	barrier->end();
	dragon->die();
	_eventDispatcher->removeAllEventListeners();
	this->unscheduleAllCallbacks();
	delete barrier;
	delete dragon;

	gameover();

	return true;
}

void EasyScene::gameover() {
	// gameover��ʾ
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* gameover = Sprite::create("gameover.png");
	gameover->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(gameover, 3);

	auto restart = MenuItemFont::create("Restart", [](Ref *pSender) {
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.8, EasyScene::createScene
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
		back->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - restart->getContentSize().height / 2 - back->getContentSize().height / 2 - 120));
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	nameInput = TextField::create("name", "arial", 24);
	if (nameInput) {
		nameInput->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - nameInput->getContentSize().height / 2 - 200));
		this->addChild(nameInput, 1);
	}

	auto upload = Label::createWithSystemFont("Upload Score", "arial", 24);
	auto uploadItem = MenuItemLabel::create(upload, CC_CALLBACK_1(EasyScene::uploadScore, this));
	uploadItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 - nameInput->getContentSize().height / 2 - 200));

	auto menu = Menu::create(restart, back, uploadItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void EasyScene::uploadScore(cocos2d::Ref * pSender) {
	// ��¼����
	if (database->getIntegerForKey("easybest") < score) {
		database->setIntegerForKey("easybest", score);
	}
	// �ϴ�������Ϸ����
	HttpRequest* request = new HttpRequest();
	request->setUrl("127.0.0.1:8080");
	request->setRequestType(HttpRequest::Type::POST);

	// write the post data
	std::string str;
	if (!nameInput->getString().empty()) {
		str = "{\"name\":\"" + nameInput->getString() + "\",\"score\":\"" + Value(score).asString() + "\"}";
	}
	else {
		str = "{\"name\":\"" + (std::string)"player" + "\",\"score\":\"" + Value(score).asString() + "\"}";
	}
	request->setRequestData(str.c_str(), str.length());
	HttpClient::getInstance()->enableCookies(NULL);
	request->setTag("Upload Score");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void EasyScene::down(float i)
{
	dragon->down();
}
