#pragma once
#include "cocos2d.h"
#include "Dragon.h"
#include "Barrier.h"

class EasyScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	void addListener();
	void checkAll(float f);
	bool onConcactBegin(PhysicsContact & contact);
	bool onTouchBegan(Touch *touch, Event *event);
	virtual bool init();

	CREATE_FUNC(EasyScene);
private:
	Dragon* dragon;
	Barrier* barrier;
	bool over;
};