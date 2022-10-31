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

#include "PlayerLayer.h"

USING_NS_CC;

enum Tag{
	Tag_Left,
	Tag_Right,
	Tag_Up,
	Tag_Down
};

bool PlayerLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	x = visibleSize.width + origin.x;
	y = visibleSize.height + origin.y;

	Left = false;
	Right = false;
	Up = false;
	Down = false;

    Player = Sprite::create("Player.png");
	Player->setPosition(Vec2(x / 2, y / 2));
	this->addChild(Player);

	ArrowButton[0] = Sprite::create("ArrowButton.png");
	ArrowButton[1] = Sprite::create("ArrowButton.png");
	ArrowButton[2] = Sprite::create("ArrowButton.png");
	ArrowButton[3] = Sprite::create("ArrowButton.png");

	ArrowButton[0]->setTag(Tag_Left);
	ArrowButton[1]->setTag(Tag_Right);
	ArrowButton[2]->setTag(Tag_Up);
	ArrowButton[3]->setTag(Tag_Down);

	this->addChild(ArrowButton[0]);
	this->addChild(ArrowButton[1]);
	this->addChild(ArrowButton[2]);
	this->addChild(ArrowButton[3]);

	ArrowButton[0]->setScale(2);
	ArrowButton[1]->setScale(2);
	ArrowButton[2]->setScale(2);
	ArrowButton[3]->setScale(2);

	ArrowButton[0]->setRotation(-90);
	ArrowButton[1]->setRotation(90);
	ArrowButton[2]->setRotation(0);
	ArrowButton[3]->setRotation(180);

	ArrowButton[0]->setPosition(Vec2(x / 2 - 130, 130));
	ArrowButton[1]->setPosition(Vec2(x / 2 + 130, 130));
	ArrowButton[2]->setPosition(Vec2(x / 2, 260));
	ArrowButton[3]->setPosition(Vec2(x / 2, 130));

	//for COMPUTER
	auto K_listner = EventListenerKeyboard::create();
	K_listner->onKeyPressed = CC_CALLBACK_2(PlayerLayer::onKeyPressed, this);
	K_listner->onKeyReleased = CC_CALLBACK_2(PlayerLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(K_listner, this);

	//for MOBILE
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan =
		CC_CALLBACK_2(PlayerLayer::onTouchesBegan, this);
	listener->onTouchesMoved =
		CC_CALLBACK_2(PlayerLayer::onTouchesMoved, this);
	listener->onTouchesEnded =
		CC_CALLBACK_2(PlayerLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	/*
	{	//플레이어 움직이고 있는 애니매이션
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.3f);

		animation->addSpriteFrameWithFile("Sprite_Walk0.png");
		animation->addSpriteFrameWithFile("Sprite_Walk1.png");
		animation->addSpriteFrameWithFile("Sprite_Walk2.png");

		animate1 = Animate::create(animation);
		animate1->retain();
	}

	{	//플레이어 서있는 애니매이션
		auto animation = Animation::create();
		animation->setDelayPerUnit(0.3f);

		animation->addSpriteFrameWithFile("Sprite_Stand0.png");
		animation->addSpriteFrameWithFile("Sprite_Stand1.png");
		animation->addSpriteFrameWithFile("Sprite_Stand2.png");

		animate2 = Animate::create(animation);
		animate2->retain();
	}

	*/
	this->schedule(schedule_selector(PlayerLayer::Player_Move));

    return true;
}

void PlayerLayer::Player_Move(float dt)
{
	auto moveStepX = 3;
	auto moveStepY = 3;

	if (Left) {
		moveStepX = -3;
		Player->setFlippedX(false);
		Player->setPosition(Player->getPositionX()+ moveStepX, Player->getPositionY());
	}
	if (Right) {
		moveStepX = 3;
		Player->setFlippedX(true);
		Player->setPosition(Player->getPositionX() + moveStepX, Player->getPositionY());
	}
	if (Up) {
		moveStepY = 3;
		Player->setPosition(Player->getPositionX(), Player->getPositionY() + moveStepY);
	}
	if (Down) {
		moveStepY = -3;
		Player->setPosition(Player->getPositionX(), Player->getPositionY() + moveStepY);
	}
}

void PlayerLayer::Player_Walk()
{
	//Player->stopAllActions();
	//Player->runAction(RepeatForever::create(animate1));
}

void PlayerLayer::Player_Stand()
{
	//Player->stopAllActions();
	//Player->runAction(RepeatForever::create(animate2));
}


void PlayerLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
		Left = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
		Down = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
		Right = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
		Up = true;
}

void PlayerLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
		Left = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
		Down = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
		Right = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
		Up = false;
}

void PlayerLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchPoint = touch->getLocation();

		auto spr1 = (cocos2d::Sprite*)this->getChildByTag(Tag_Left);
		auto spr2 = (cocos2d::Sprite*)this->getChildByTag(Tag_Up);
		auto spr3 = (cocos2d::Sprite*)this->getChildByTag(Tag_Right);
		auto spr4 = (cocos2d::Sprite*)this->getChildByTag(Tag_Down);

		Rect sprRect1 = spr1->getBoundingBox();
		Rect sprRect2 = spr2->getBoundingBox();
		Rect sprRect3 = spr3->getBoundingBox();
		Rect sprRect4 = spr4->getBoundingBox();

		if (sprRect1.containsPoint(touchPoint))
		{
			Left = true;
			return;
		}
		if (sprRect2.containsPoint(touchPoint))
		{
			Up = true;
			return;
		}
		if (sprRect3.containsPoint(touchPoint))
		{
			Right = true;
			return;
		}
		if (sprRect4.containsPoint(touchPoint))
		{
			Down = true;
			return;
		}
	}

}

void PlayerLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto touchPoint = touch->getLocation();

		auto spr1 = (cocos2d::Sprite*)this->getChildByTag(Tag_Left);
		auto spr2 = (cocos2d::Sprite*)this->getChildByTag(Tag_Up);
		auto spr3 = (cocos2d::Sprite*)this->getChildByTag(Tag_Right);
		auto spr4 = (cocos2d::Sprite*)this->getChildByTag(Tag_Down);

		Rect sprRect1 = spr1->getBoundingBox();
		Rect sprRect2 = spr2->getBoundingBox();
		Rect sprRect3 = spr3->getBoundingBox();
		Rect sprRect4 = spr4->getBoundingBox();

		if (sprRect1.containsPoint(touchPoint))
		{
			Left = true;
			Up = false;
			Right = false;
			Down = false;
			return;
		}
		if (sprRect2.containsPoint(touchPoint))
		{
			Up = true;
			Left = false;
			Right = false;
			Down = false;
			return;
		}
		if (sprRect3.containsPoint(touchPoint))
		{
			Right = true;
			Left = false;
			Up = false;
			Down = false;
			return;
		}
		if (sprRect4.containsPoint(touchPoint))
		{
			Down = true;
			Left = false;
			Up = false;
			Right = false;
			return;
		}
	}

}

void PlayerLayer::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	Left = false;
	Up = false;
	Right = false;
	Down = false;
}
void PlayerLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	Left = false;
	Up = false;
	Right = false;
	Down = false;
}