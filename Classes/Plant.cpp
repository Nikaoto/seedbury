#include <iostream>
#include "Plant.h"
#include "Land.h"

const cocos2d::Size Plant::SIZE = cocos2d::Size(128, 128);
const std::string Plant::TEXTURE_PATHS[4] = {
    "plant_0.png",
    "plant_1.png",
    "plant_2.png",
    "plant_3.png"
};

using namespace cocos2d;

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
