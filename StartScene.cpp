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

#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include"ui/CocosGUI.h"
#include"cocos2d.h"
#include"Resouce.h"
#include"cocostudio/CocoStudio.h"
#include "VisibleRect.h"
USING_NS_CC;
using namespace ui;

Scene* StartScene::createScene()
{
    return StartScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StartSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto csbNode = CSLoader::createNode("E:/CocosStudioProject/scene/scene/MainScene.csb");
    this->addChild(csbNode);

    auto button = ui::Button::create(s_pathResume_nor, s_pathResume_pressed, s_pathResume_pressed);
    float x = origin.x + visibleSize.width/2 ;
    float y = origin.y + visibleSize.height / 2;
    button->setPosition(Vec2(x, y));
    float width = 38;
    float height = 41;
    Size buttonSize(width, height);
    //notes:这里要把ignoresize属性设置成false,才能调整大小
    button->ignoreContentAdaptWithSize(false);
    button->setContentSize(buttonSize);
    auto root = dynamic_cast<ui::Widget*>(csbNode->getChildByName("Panel_1"));
    root->addChild(button, 1);

    auto image = ui::Helper::seekWidgetByName(root, "Image_6");
    image->setVisible(false);
    button->addClickEventListener(CC_CALLBACK_1(StartScene::startPressedCallBack, this));

    auto lbl_startgame = ui::Scale9Sprite::create(s_pathName);
    lbl_startgame->setPosition(VisibleRect::top());
    root->addChild(lbl_startgame,1,"lbl_startgame");
    return true;
}


void StartScene::startPressedCallBack(Ref* pSender)
{
    Scene* gameScene = SceneController::getGameScene();

    Director::getInstance()->replaceScene(gameScene);


}
