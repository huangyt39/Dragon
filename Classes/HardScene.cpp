#include "HardScene.h"
#include "ui\CocosGUI.h"

using namespace cocos2d::ui;

cocos2d::Scene * HardScene::createScene() {
	return HardScene::create();
}

bool HardScene::init() {
	if (!Scene::init()) {
		return false;

		return true;
	}
}
