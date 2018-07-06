#pragma once
#include "cocos2d.h"

class EasyScene : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
};