#include <iostream>
#include "Land.h"
#include "Plant.h"
#include "DBManager.h"

const cocos2d::Size Land::SIZE = cocos2d::Size(128, 128);
const std::string Land::FERTILE_SPRITE = "land_fertile.png";
const std::string Land::INFERTILE_SPRITE = "land_infertile.png";

using namespace cocos2d;

bool Land::init() {
    if (!Sprite::init()) {
        return false;
    }
    setTexture(INFERTILE_SPRITE);
    setAnchorPoint(Vec2(0, 1));
    setScale(SIZE.width / getContentSize().width, SIZE.height / getContentSize().height);
    
    // Set up listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (getBoundingBox().containsPoint(touch->getLocation())) {
            if (isFertile()) {
                if (plant == nullptr) {
                    plant = Plant::create();
                    plant->setPosition(0, Plant::MARGIN_BOTTOM); // TODO carry this out as plant margin
                    this->addChild(plant, 1);
                    // Milliseconds since epoch
                    unsigned long now = std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now().time_since_epoch()).count();
                    log("%li", now);
                    
                    DBManager::getInstance()->savePlant(this->landNumber, now, "standard");
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

const bool Land::isFertile() {
    return this->fertile;
}

void Land::setLandNumber(int landNumber) {
    this->landNumber = landNumber;
}

const int Land::getLandNumber() {
    return this->landNumber;
}
