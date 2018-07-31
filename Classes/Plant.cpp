#include <iostream>
#include "Plant.h"
#include "Land.h"

// Constants
const cocos2d::Size Plant::SIZE = cocos2d::Size(128, 128);
const std::string Plant::TEXTURE_PATHS[4] = {
    "plant_0.png",
    "plant_1.png",
    "plant_2.png",
    "plant_3.png"
};
const int Plant::GROWTH_TIME = 5;
const int Plant::MIN_GROWTH_STAGE = 0;
const int Plant::MAX_GROWTH_STAGE = 3;
const int Plant::MARGIN_BOTTOM = 55;

using namespace cocos2d;

Plant* Plant::create(unsigned long plantTime, std::string plantType) {
    Plant *pRet = new(std::nothrow) Plant(plantTime, plantType);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

Plant::Plant(unsigned long plantTime, std::string plantType) {
    this->plantTime = plantTime;
    this->plantType = plantType;
    
    if (plantTime != 0) {
        // Calculate growthStage
        unsigned long now = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
        auto elapsedSeconds = (now - plantTime) / 1000;
        setGrowthStage((int)std::floor(elapsedSeconds / GROWTH_TIME));
    }
}

bool Plant::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    setTexture(TEXTURE_PATHS[0]);
    setAnchorPoint(Vec2(0, 0));
    time = GROWTH_TIME;
    
    scheduleUpdate();
    
    return true;
}

void Plant::update(float dt) {
    if (getGrowthStage() < MAX_GROWTH_STAGE) {
        time -= dt;
        if (time <= 0) {
            setGrowthStage(getGrowthStage() + 1);
            time = GROWTH_TIME;
        }
    }
}

void Plant::setGrowthStage(const int stage) {
    if (stage < MIN_GROWTH_STAGE) {
        log("%s", "setGrowthStage: stage is lower than allowed minimum!");
        this->growthStage = MIN_GROWTH_STAGE;
    } else if (stage > MAX_GROWTH_STAGE) {
        log("%s", "setGrowthStage: stage is higher than allowed maximum!");
        this->growthStage = MAX_GROWTH_STAGE;
    } else {
        this->growthStage = stage;
    }
    // Update texture
    this->setTexture(TEXTURE_PATHS[growthStage]);
}

const int Plant::getGrowthStage() {
    return this->growthStage;
}

void Plant::setPlantTime(const unsigned long plantTime){
    this->plantTime = plantTime;
}

const unsigned long Plant::getPlantTime() {
    return this->plantTime;
}

void Plant::setPlantType(const std::string plantType){
    this->plantType = plantType;
}

const std::string Plant::getPlantType() {
    return this->plantType;
}
