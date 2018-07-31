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
    
    // Init
    static Plant* create(unsigned long plantTime, std::string plantType);
    Plant(unsigned long plantTime = 0, std::string plantType = "standard");
    bool init() override;

    // Functions
    void update(float dt) override;

    // Temporary
    float time;

    // Getters and setters
    const int getGrowthStage();
    void setGrowthStage(const int stage);

    const unsigned long getPlantTime();
    void setPlantTime(const unsigned long plantTime);

    const std::string getPlantType();
    void setPlantType(const std::string plantType);
protected:
    unsigned long plantTime;
    std::string plantType;
    int growthStage;
};
