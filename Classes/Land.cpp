#include <iostream>
#include "Land.h"

using namespace cocos2d;

bool Land::init() {
    if (!Node::init()) {
        return false;
    }
    
    this->setContentSize(LAND_SIZE);

    // Draw land
    Vec2 rect[4] = {
        Vec2(0, 0),
        Vec2(LAND_SIZE.width, 0),
        Vec2(LAND_SIZE.width, LAND_SIZE.height),
        Vec2(0, LAND_SIZE.height)
    };
    this->drawNode = DrawNode::create();
    Color4F color;
    if (this->isFertile()) {
        color = Color4F(0.65f, 0.16f, 0.16f, 1);
    } else {
        color = Color4F(0.40f, 0.31f, 0.31f, 1);
    }
    
    this->drawNode->drawPolygon(rect, 4, color, 1.5f, Color4F::BLACK);
    this->addChild(this->drawNode);
    //
    
    // Set up listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (this->getBoundingBox().containsPoint(touch->getLocation())) {
            log("%s", "TOUCH");
        }
        return true;
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void Land::setFertile(bool fertile){
    this->fertile = fertile;
    if (fertile) {
        this->drawNode->setColor(Color3B::WHITE);
    } else {
        this->drawNode->setColor(Color3B::BLACK);
    }
}

bool Land::isFertile() {
    return this->fertile;
}
