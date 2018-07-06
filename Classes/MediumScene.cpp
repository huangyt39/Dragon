#include "MediumScene.h"
#include "ui\CocosGUI.h"

using namespace cocos2d::ui;

cocos2d::Scene * MediumScene::createScene() {
	return MediumScene::create();
}

bool MediumScene::init() {
	if (!Scene::init()) {
		return false;

		return true;
	}
}
