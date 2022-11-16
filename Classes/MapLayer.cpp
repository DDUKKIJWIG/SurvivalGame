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

#include "MapLayer.h"
#include "SimplexNoise.h"

USING_NS_CC;

enum Tag {
	Tag_Left,
	Tag_Right,
	Tag_Up,
	Tag_Down
};

bool MapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	x = visibleSize.width + origin.x;
	y = visibleSize.height + origin.y;

	//저장된 맵 데이터 없어유
	currentX = MAPSIZE / 2;
	currentY = MAPSIZE / 2;

	posX = 0;
	posY = 0;

	//Sprite::createWithSpriteFrameName("jumpman0001.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Tile.plist", "Tile.png");

	Left = false;
	Right = false;
	Up = false;
	Down = false;

	mapNode = Node::create();
	this->addChild(mapNode);

	Player = Sprite::create("Player.png");
	Player->setPosition(Vec2(x / 2, y / 2));
	this->addChild(Player, 100);

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

	//컴퓨터
	auto K_listner = EventListenerKeyboard::create();
	K_listner->onKeyPressed = CC_CALLBACK_2(MapLayer::onKeyPressed, this);
	K_listner->onKeyReleased = CC_CALLBACK_2(MapLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(K_listner, this);

	//모바일
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan =
		CC_CALLBACK_2(MapLayer::onTouchesBegan, this);
	listener->onTouchesMoved =
		CC_CALLBACK_2(MapLayer::onTouchesMoved, this);
	listener->onTouchesEnded =
		CC_CALLBACK_2(MapLayer::onTouchesEnded, this);
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
	createMap(currentX, currentY);
	setMapPos(currentX, currentY);

	createMap(currentX, currentY + 1);
	setMapPos(currentX, currentY + 1);

	createMap(currentX, currentY - 1);
	setMapPos(currentX, currentY - 1);

	createMap(currentX + 1, currentY);
	setMapPos(currentX + 1, currentY);

	createMap(currentX + 1, currentY + 1);
	setMapPos(currentX + 1, currentY + 1);

	createMap(currentX + 1, currentY - 1);
	setMapPos(currentX + 1, currentY - 1);

	createMap(currentX - 1, currentY);
	setMapPos(currentX - 1, currentY);

	createMap(currentX - 1, currentY + 1);
	setMapPos(currentX - 1, currentY + 1);

	createMap(currentX - 1, currentY - 1);
	setMapPos(currentX - 1, currentY - 1);


	this->schedule(schedule_selector(MapLayer::Background));

    return true;
}

void MapLayer::createMap(int posX, int posY)
{
	if (mapLayer_[posX][posY] == NULL)
	{
		mapLayer_[posX][posY] = Layer::create();
		mapNode->addChild(mapLayer_[posX][posY]);
	}

	if (mapData_[posX][posY] == NULL)
	{
		mapData_[posX][posY] = MapData::create();
		this->addChild(mapData_[posX][posY]);

		for (int indexX = 0; indexX < TILESIZE / 2; indexX++)
		{
			for (int indexY = 0; indexY < TILESIZE; indexY++)
			{
				float a = SimplexNoise::noise(indexX * 0.1, indexY * 0.1);

				if (a < 0.5)
				{
					mapData_[posX][posY]->mapSprite[indexX][indexY] = Sprite::createWithSpriteFrameName("Tile1.png");
				
					if (indexX == TILESIZE / 2 - 1 || indexY == TILESIZE - 1 || indexX == 0 || indexY == 0)
						mapData_[posX][posY]->mapSprite[indexX][indexY]->setColor(Color3B::BLACK);

					mapData_[posX][posY]->mapSprite[indexX][indexY]->setPosition(Vec2(SIZE * indexX, SIZE * indexY));
					mapLayer_[posX][posY]->addChild(mapData_[posX][posY]->mapSprite[indexX][indexY]);

				}
				else
				{
					mapData_[posX][posY]->mapSprite[indexX][indexY] = Sprite::createWithSpriteFrameName("Tile2.png");
					mapData_[posX][posY]->mapSprite[indexX][indexY]->setPosition(Vec2(SIZE * indexX, SIZE * indexY));
					mapLayer_[posX][posY]->addChild(mapData_[posX][posY]->mapSprite[indexX][indexY]);
				}
			}
		}
	}
	

}

void MapLayer::removeMap(int posX, int posY)
{
	if (mapLayer_[posX][posY] != NULL)
		mapNode->removeChild(mapLayer_[posX][posY]);
	if(mapData_[posX][posY] != NULL)
		this->removeChild(mapData_[posX][posY]);
}

void MapLayer::setMapPos(int posX, int posY)
{
	if (mapLayer_[posX][posY] != NULL)
	{
		if(posX == currentX)

	}



	/*	mapLayer_[currentX - 1][currentY]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() - SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY()));
		}
		if (mapLayer_[currentX + 1][currentY] != NULL)//+ 0
		{
			mapLayer_[currentX + 1][currentY]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() + SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY()));
		}
		if (mapLayer_[currentX - 1][currentY - 1] != NULL)//- -
		{
			mapLayer_[currentX - 1][currentY - 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() - SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY() - SIZE * TILESIZE));
		}
		if (mapLayer_[currentX - 1][currentY + 1] != NULL)//- +
		{
			mapLayer_[currentX - 1][currentY + 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() - SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY() + SIZE * TILESIZE));
		}
		if (mapLayer_[currentX][currentY + 1] != NULL)//0 +
		{
			mapLayer_[currentX][currentY + 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX(),
				mapLayer_[currentX][currentY]->getPositionY() + SIZE * TILESIZE));
		}
		if (mapLayer_[currentX][currentY - 1] != NULL)//0 -
		{
			mapLayer_[currentX][currentY - 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX(),
				mapLayer_[currentX][currentY]->getPositionY() - SIZE * TILESIZE));
		}
		if (mapLayer_[currentX + 1][currentY + 1] != NULL)//+ +
		{
			mapLayer_[currentX + 1][currentY + 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() + SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY() + SIZE * TILESIZE));
		}
		if (mapLayer_[currentX + 1][currentY - 1] != NULL)//+ -
		{
			mapLayer_[currentX + 1][currentY - 1]->setPosition(Vec2(mapLayer_[currentX][currentY]->getPositionX() + SIZE * TILESIZE / 2,
				mapLayer_[currentX][currentY]->getPositionY() - SIZE * TILESIZE));
		}
	}*/
}

void MapLayer::Background(float dt)
{
	int moveStepX = 3;
	int moveStepY = 3;

	if (Left) {
		moveStepX = 3;
		mapNode->setPosition(int(mapNode->getPositionX()) + moveStepX,int(mapNode->getPositionY()));
	}
	if (Right) {
		moveStepX = -3;
		mapNode->setPosition(int(mapNode->getPositionX()) + moveStepX,int(mapNode->getPositionY()));
	}
	if (Up) {
		moveStepY = -3;
		mapNode->setPosition(int(mapNode->getPositionX()), int(mapNode->getPositionY() + moveStepY));
	}
	if (Down) {
		moveStepY = 3;
		mapNode->setPosition(int(mapNode->getPositionX()),int(mapNode->getPositionY() + moveStepY));
	}

	if (int(mapNode->getPositionX()) > (SIZE * TILESIZE / 4 + TILESIZE) + SIZE * TILESIZE / 2 * posX)// 왼쪽
	{
		createMap(currentX - 2, currentY);
		removeMap(currentX + 1, currentY);

		posX += 1;
		currentX -= 1;
		//setMapPos();

		log("LEFT");
	}
	else if (int(mapNode->getPositionX()) < (-SIZE * TILESIZE / 4 - TILESIZE) + SIZE * TILESIZE / 2 * posX )// 오른쪽
	{
		createMap(currentX + 2, currentY);
		removeMap(currentX - 1, currentY);

		posX -= 1;
		currentX += 1;
		//setMapPos();

		log("RIGHT");
	}
	else if (mapNode->getPositionY() > (SIZE * TILESIZE / 2 ) -(2 * TILESIZE) - TILESIZE * posY)// 아래쪽
	{
		log("DOWN");

	}
	else if (mapNode->getPositionY() < (-(SIZE * TILESIZE / 2) - TILESIZE) -(2 * TILESIZE) + TILESIZE * posY)// 위쪽
	{
		log("UP");

	}
}

void MapLayer::Player_Walk()
{
	//Player->stopAllActions();
	//Player->runAction(RepeatForever::create(animate1));
}

void MapLayer::Player_Stand()
{
	//Player->stopAllActions();
	//Player->runAction(RepeatForever::create(animate2));
}


void MapLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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

void MapLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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

void MapLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
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

void MapLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
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

void MapLayer::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	Left = false;
	Up = false;
	Right = false;
	Down = false;
}
void MapLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	Left = false;
	Up = false;
	Right = false;
	Down = false;
}