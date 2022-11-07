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

enum Tag{

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
	//Sprite::createWithSpriteFrameName("jumpman0001.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Tile.plist", "Tile.png");

	creatMap(currentX, currentY);

    return true;
}

void MapLayer::creatMap(int posX, int posY)
{
	if (mapLayer_[posX][posY] == NULL)
	{
		mapLayer_[posX][posY] = Layer::create();
		this->addChild(mapLayer_[posX][posY]);

		if (mapData_[posX][posY] == NULL)
		{
			mapData_[posX][posY] = MapData::create();
			this->addChild(mapData_[posX][posY]);

		}
	}

	for (int indexX = 0; indexX < TILESIZE / 2; indexX++)
	{
		for (int indexY = 0; indexY < TILESIZE; indexY++)
		{
			mapData_[posX][posY]->mapSprite[indexX][indexY] = Sprite::createWithSpriteFrameName("Tile1.png");
			mapData_[posX][posY]->mapSprite[indexX][indexY]->setPosition(Vec2(SIZE * indexX, SIZE * indexY));
			mapLayer_[posX][posY]->addChild(mapData_[posX][posY]->mapSprite[indexX][indexY]);
		}
	}
}