#pragma once
#include "cocos2d.h"

const static cocos2d::Size PLANT_SIZE = cocos2d::Size(128, 128);
const static std::vector<std::string> TEXTURE_PATHS = {
    "plant_0.png",
    "plant_1.png",
    "plant_2.png",
    "plant_3.png"
};

class Plant : public cocos2d::Sprite {
public:
    static const int GROWTH_TIME = 5;
    static const int MIN_GROWTH_STAGE = 0;
    static const int MAX_GROWTH_STAGE = 3;
    static const int MARGIN_BOTTOM = 55;
    
    bool init() override;
    void update(float dt) override;
    
    float time;

    const int getGrowthStage();
    void setGrowthStage(const int stage);

    CREATE_FUNC(Plant);
protected:
    int growthStage;
};
