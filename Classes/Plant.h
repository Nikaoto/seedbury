#pragma once
#include "cocos2d.h"

const static cocos2d::Size PLANT_SIZE = cocos2d::Size(128, 128);
const static int MIN_GROWTH_STAGE = 0;
const static int MAX_GROWTH_STAGE = 3;
const static std::vector<std::string> TEXTURE_PATHS = {
    "plant_0.png",
    "plant_1.png",
    "plant_2.png",
    "plant_3.png"
};
const static int GROWTH_TIME = 5;

class Plant : public cocos2d::Sprite {
public:
    bool init() override;
    void update(float dt) override;
    
    float time;

    const int getGrowthStage();
    void setGrowthStage(const int stage);

    CREATE_FUNC(Plant);
protected:
    int growthStage;
};
