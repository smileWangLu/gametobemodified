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

#include "MyScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include"ui/CocosGUI.h"
#include "MyNode.h"
#include"cocos2d.h"
#include"cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;

Scene* MyScene::createScene()
{
    return MyScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MyScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
#pragma region
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto action = CSLoader::createTimeline("scene/MainScene.csb");
    action->setTimeSpeed(2.0f);
    action->gotoFrameAndPlay(1, true);

    auto csbNode = CSLoader::createNode("E:/CocosStudioProject/scene/scene/MainScene.csb");
    this->addChild(csbNode);
    
    csbNode->runAction(action);
    
    auto root = dynamic_cast<ui::Widget*>(csbNode->getChildByName("container"));
    auto image = ui::Helper::seekWidgetByName(root, "Image_1_0");
    if (image)
        image->setColor(Color3B(0, 255, 0));
   // csbNode->setPosition(Vec2(origin.x, origin.y));

#pragma endregion
    return true;
}


