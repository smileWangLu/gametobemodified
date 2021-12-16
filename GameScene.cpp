/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include"ui/CocosGUI.h"
#include"cocos2d.h"
#include"Resouce.h"
#include"cocostudio/CocoStudio.h"
#include "VisibleRect.h"
#include"GameController.h"
USING_NS_CC;
using namespace ui;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto csbNode = CSLoader::createNode("E:/CocosStudioProject/scene/scene/MainScene.csb");
	this->addChild(csbNode);
	auto root = dynamic_cast<ui::Widget*>(csbNode->getChildByName("Panel_1"));

	
	auto sprite_hero = Sprite::create(s_pathHero1);
	sprite_hero->setPosition(VisibleRect::bottom());
	sprite_hero->setScale((2.0f));
	sprite_hero->setContentSize(Size(1, 3));
	auto animation = Animation::create();
	
	animation->addSpriteFrameWithFile(s_pathHero1);
	animation->addSpriteFrameWithFile(s_pathHero2);
	animation->setDelayPerUnit(1.0f / 2.0f);
	animation->setRestoreOriginalFrame(true);

	auto action = Animate::create(animation);
	auto seq = Sequence::create(action,nullptr);
	sprite_hero->runAction(RepeatForever::create(seq));
	this->addChild(sprite_hero, 1, "hero");

	image_pause = ui::Helper::seekWidgetByName(root, "Image_6");
	image_pause->setTouchEnabled(true);
	image_pause->addClickEventListener(CC_CALLBACK_1(GameScene::pausePressedCallback, this));
	

	std::string score = "0";
	auto lbl_score = Label::createWithTTF(score, "font/Marker-Felt.ttf", 40.0f);
	lbl_score->setPosition(VisibleRect::leftBottom() + Vec2(50, 0));
	this->addChild(lbl_score,1,"label");

	auto button_bomb = ui::Button::create(s_pathBomb);
	button_bomb->setPosition(VisibleRect::leftBottom());
	root->addChild(button_bomb, 1);
	
	button_resumeplay = ui::Button::create(s_pathResume_nor);
	button_resumeplay->setPosition(VisibleRect::center());
	button_resumeplay->loadTexturePressed(s_pathResume_pressed);
	button_resumeplay->setEnabled(false);
	button_resumeplay->setVisible(false);
	button_resumeplay->addClickEventListener(CC_CALLBACK_1(GameScene::resumePressedCallback, this));
	button_resumeplay->setScale(2.0f);
	root->addChild(button_resumeplay, 1);
	return true;
}

void GameScene::onEnter() {
	Scene::onEnter();
	startGame();
	startShoot();
}
//先不加关卡数
void GameScene::startGame() {
	GameController::instance()->startGame(this);
}
void GameScene::startShoot() {
	this->schedule(CC_SCHEDULE_SELECTOR(GameController::shooting),0.0f);
}

void GameScene::pausePressedCallback(Ref* pSender)
{
	Director::getInstance()->pause();
	button_resumeplay->setEnabled(true);
	button_resumeplay->setVisible(true);
	image_pause->setEnabled(false);
}
void GameScene::resumePressedCallback(Ref* pSender)
{
	Director::getInstance()->resume();
	button_resumeplay->setEnabled(false);
	button_resumeplay->setVisible(false);
	image_pause->setEnabled(true);
}


