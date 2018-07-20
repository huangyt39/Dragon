#pragma once
#include "cocos2d.h"
#include "Dragon.h"

class EasyScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(EasyScene);
};