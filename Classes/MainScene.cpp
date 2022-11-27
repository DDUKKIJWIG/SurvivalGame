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

#include "MainScene.h"
#include "MapLayer.h"
#include "PlayerLayer.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

bool MainScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	else
	{
		Roading();

		cocos2d::Director::getInstance()->setClearColor(Color4F::GRAY);

		auto Map_Layer_ = MapLayer::create();
		this->addChild(Map_Layer_);

		//auto Player_Layer_ = PlayerLayer::create();
		//this->addChild(Player_Layer_);

		return true;
	}
}

void MainScene::Roading()
{
	Director::getInstance()->getTextureCache()->
		addImageAsync("Tile.png", CC_CALLBACK_1(MainScene::Road_Resources, this));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Tile.plist", "Tile.png");
}

void MainScene::Road_Resources(cocos2d::Texture2D* texture)
{

}
