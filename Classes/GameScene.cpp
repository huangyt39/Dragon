#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	return Scene::create();
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first

	if (!Scene::init())
	{
		return false;
	}

	//background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("background.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height /2 + origin.y));
	this->addChild(bg, 0);

	//test
	auto texture = Director::getInstance()->getTextureCache()->addImage("blackdra_fly.png");
	auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 135, 135)));

	bla.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(135 * i, 0, 135, 135)));
		bla.pushBack(frame);
	}

	auto blaAnimation = Animation::createWithSpriteFrames(bla, 0.1f, 1);
	auto blaAnimate = Animate::create(blaAnimation);

	//mouse
	this->mouse = Sprite::create();
	this->mouse->runAction(RepeatForever::create(blaAnimate));
	this->mouse->setPosition(Vec2(visibleSize.width / 2 +origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(this->mouse);

	return true;
}
