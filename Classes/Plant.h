#pragma once
#include "cocos2d.h"

class Plant : public cocos2d::Sprite {
public:
    static const cocos2d::Size SIZE;
    static const std::string TEXTURE_PATHS[4];
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
