#include <iostream>
#include "Land.h"

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
            log("%s", "TOUCH");
            setFertile(!isFertile());
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
