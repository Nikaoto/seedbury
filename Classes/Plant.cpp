#include "Plant.h"
#include "Land.h"
#include "timeutil.h"

// Constants
const cocos2d::Size Plant::SIZE = cocos2d::Size(128, 128);
const int Plant::MIN_GROWTH_STAGE = 0;
const int Plant::MAX_GROWTH_STAGE = 3;
const int Plant::MARGIN_BOTTOM = 55;

const std::string Plant::PLANT_TYPES[2] = {
    "Eggplant", "Tomato"
};

// Init global plant data for each type of plant
const std::map<std::string, PlantData> Plant::PLANT_DATA = {
    {PLANT_TYPES[0], PlantData(PLANT_TYPES[0], 5)},
    {PLANT_TYPES[1], PlantData(PLANT_TYPES[1], 3)}
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
    // Glow if last stage
    if (this->growthStage == MAX_GROWTH_STAGE) {
        // Add glow sprite behind this
        auto glowSprite = Sprite::create("glow.png");
        glowSprite->setAnchorPoint(Vec2(0, 0));
        glowSprite->setPosition(0, 0);
        this->addChild(glowSprite, -1);
    }
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
