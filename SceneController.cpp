
#include "SceneController.h"

#include "cocos2d.h"

USING_NS_CC;
Scene* SceneController::getGameScene() {
	return GameScene::createScene();
}


