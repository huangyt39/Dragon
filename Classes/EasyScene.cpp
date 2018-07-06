#include "EasyScene.h"
#include "ui\CocosGUI.h"

using namespace cocos2d::ui;

cocos2d::Scene * EasyScene::createScene() {
	return EasyScene::create();
}

bool EasyScene::init() {
	if (!Scene::init()) {
		return false;

		return true;
	}
}
