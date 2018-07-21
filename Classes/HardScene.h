#pragma once
#include "cocos2d.h"
#include "Dragon.h"
#include "Barrier.h"

class HardScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();
	void addListener();
	void checkAll(float f);
	bool onConcactBegin(PhysicsContact & contact);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void barrierMove(char c);
	void dargonMove(char c);
	virtual bool init();

	CREATE_FUNC(HardScene);
private:
	Dragon * dragon;
	Barrier* barrier;
	bool DMove;
	bool BMove;
	char Dmovekey;
	char Bmovekey;
};