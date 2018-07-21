#pragma once
#include "cocos2d.h"
#include "Dragon.h"
#include "Barrier.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "ui\CocosGUI.h"

using namespace std;
using namespace CocosDenshion;

class EasyScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	void addListener();
	void checkAll(float f);
	bool onConcactBegin(PhysicsContact & contact);
	bool onTouchBegan(Touch *touch, Event *event);
	virtual bool init();
	void gameover();
	void uploadScore(Ref *pSender);

	CREATE_FUNC(EasyScene);
private:
	Dragon* dragon;
	Barrier* barrier;
	bool over;
	Label* Score;
	Label* Best;
	int score = 0;
};