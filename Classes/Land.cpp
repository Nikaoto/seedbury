#include <iostream>
#include "Land.h"
#include "Plant.h"

using namespace cocos2d;

bool Land::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    setTexture(INFERTILE_SPRITE);
    setAnchorPoint(Vec2(0, 1));
    setScale(LAND_SIZE.width / getContentSize().width, LAND_SIZE.height / getContentSize().height);
    
    // Set up listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (getBoundingBox().containsPoint(touch->getLocation())) {
            if (isFertile()) {
                if (plant == nullptr) {
                    plant = Plant::create();
                    plant->setPosition(0, 55); // TODO carry this out as plant margin
                    this->addChild(plant, 1);
                } else {
                    plant->setGrowthStage(plant->getGrowthStage() + 1);
                }
            } else {
                setFertile(true);
            }
        }
        return true;
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void Land::setFertile(bool fertile){
    this->fertile = fertile;
    // Update sprite
    if (fertile) {
        setTexture(FERTILE_SPRITE);
    } else {
        setTexture(INFERTILE_SPRITE);
    }
}

bool Land::isFertile() {
    return this->fertile;
}
