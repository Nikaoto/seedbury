#include <iostream>
#include "Plant.h"
#include "Land.h"
#include "timeutil.h"

// Constants
const cocos2d::Size Plant::SIZE = cocos2d::Size(128, 128);
const int Plant::MIN_GROWTH_STAGE = 0;
const int Plant::MAX_GROWTH_STAGE = 3;
const int Plant::MARGIN_BOTTOM = 55;

struct PlantData {
    std::string type;
    int growthTime;
    std::string* texturePaths;
    PlantData(std::string type, int growthTime, std::string* texturePaths) {
        this->type = type;
        this->growthTime = growthTime;
        this->texturePaths = texturePaths;
    }
};

const std::map<const std::string, const PlantData> PLANT_DATA = {
    {"Standard", PlantData("Standard", 10, new std::string[4] {"plant_0.png", "plant_1.png", "plant_2.png", "plant_3.png"})},
    {"Tomato", PlantData("Tomato", 3, new std::string[4] {"plant_0.png", "plant_tomato_1.png", "plant_tomato_2.png", "plant_tomato_3.png"})}
};

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
    
    this->growthTime = PLANT_DATA.at(plantType).growthTime;
    this->texturePaths = PLANT_DATA.at(plantType).texturePaths;
}

bool Plant::init() {
    if (!Sprite::init()) {
        return false;
    }
    
    setGrowthStage(0);
    setTexture(this->texturePaths[0]);
    setAnchorPoint(Vec2(0, 0));
    
    scheduleUpdate();
    
    return true;
}

void Plant::update(float dt) {
    // Update growth stage
    if (getGrowthStage() < MAX_GROWTH_STAGE) {
        unsigned long now = timeutil::getEpochSeconds();
        int elapsedSeconds = (int) (now - plantTime);
        if (elapsedSeconds > 0) {
            int stagesPassed = std::floor(elapsedSeconds / this->growthTime);
            setGrowthStage(stagesPassed);
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
    this->setTexture(this->texturePaths[growthStage]);
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

const std::string Plant::getPlantType() {
    return this->plantType;
}
