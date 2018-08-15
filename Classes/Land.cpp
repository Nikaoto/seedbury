#include <iostream>
#include "Land.h"
#include "Plant.h"
#include "DBManager.h"
#include "timeutil.h"
#include "ui/CocosGUI.h"

// Constants
const cocos2d::Size Land::SIZE = cocos2d::Size(128, 128);
const std::string Land::FERTILE_SPRITE = "land_fertile.png";
const std::string Land::INFERTILE_SPRITE = "land_infertile.png";

using namespace cocos2d;

Land* Land::create(int landNumber) {
    Land *pRet = new(std::nothrow) Land(landNumber);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

Land::Land(int landNumber) {
    this->landNumber = landNumber;
}

bool Land::init() {
    if (!Sprite::init()) {
        return false;
    }
    setTexture(INFERTILE_SPRITE);
    setAnchorPoint(Vec2(0, 1));
    setScale(SIZE.width / getContentSize().width, SIZE.height / getContentSize().height);
    // Load plant from db
    auto p = DBManager::getInstance()->getPlant(landNumber);
    if (p != nullptr) {
        setFertile(true);
        setPlant(p);
    } else {
        plant = nullptr;
        fertile = false;
    }

    // Set up listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (getBoundingBox().containsPoint(touch->getLocation())) {
            if (isFertile()) {
				if (plant != nullptr && plant->getGrowthStage() == plant->MAX_GROWTH_STAGE) {
				    CCLOG("HARVEST");
				} else if (plant == nullptr) {
				    CCLOG("OPEN PLANT MENU from plant %i", this->getLandNumber());
                    this->plantMenuCallback(this->getLandNumber());
                } else {
                    // open plant info or removal
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

void Land::plantPlant(std::string plantType) {
    // Create new plant object
    setPlant(Plant::create(timeutil::getEpochSeconds(), plantType));
    // Save to db
    DBManager::getInstance()->savePlant(this->landNumber, plant->getPlantTime(), plant->getPlantType());
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

void Land::setLandNumber(const int landNumber) {
    this->landNumber = landNumber;
}

const int Land::getLandNumber() {
    return this->landNumber;
}

void Land::setPlant(Plant* plant) {
    if (plant != nullptr) {
        this->plant = &(*plant);
        this->plant->setPosition(0, Plant::MARGIN_BOTTOM);
        this->addChild(this->plant, 1);
    }
}

const Plant* Land::getPlant() {
    return this->plant;
}

void Land::setPlantMenuCallback(std::function<void(int senderLandNumber)> callback) {
    this->plantMenuCallback = callback;
}