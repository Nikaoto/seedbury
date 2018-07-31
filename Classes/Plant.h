#pragma once
#include "cocos2d.h"

class Plant : public cocos2d::Sprite {
public:
    // Constants
    static const cocos2d::Size SIZE;
    static const std::string TEXTURE_PATHS[4];
    static const int GROWTH_TIME;
    static const int MIN_GROWTH_STAGE;
    static const int MAX_GROWTH_STAGE;
    static const int MARGIN_BOTTOM;
    
    // Cocos2d rituals
    bool init() override;
    CREATE_FUNC(Plant);

    // Functions
    void update(float dt) override;

    // Temporary
    float time;

    // Getters and setters
    const int getGrowthStage();
    void setGrowthStage(const int stage);

protected:
    int growthStage;
};
