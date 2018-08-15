#pragma once
#include "cocos2d.h"

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

class Plant : public cocos2d::Sprite {
public:
    // Constants
    static const cocos2d::Size SIZE;
    static const std::string PLANT_TYPES[2];
    static const std::map<std::string, PlantData> PLANT_DATA;
    static const int MIN_GROWTH_STAGE;
    static const int MAX_GROWTH_STAGE;
    static const int MARGIN_BOTTOM;
    
    // Init
    Plant(unsigned long plantTime = 0, std::string plantType = "Standard");
    static Plant* create(unsigned long plantTime, std::string plantType = "Standard");
    bool init() override;

    // Functions
    void update(float dt) override;

    // Getters and setters
    /// growthStage
    const int getGrowthStage();
    void setGrowthStage(const int stage);
    /// plantTime
    const unsigned long getPlantTime();
    void setPlantTime(const unsigned long plantTime);
    // plantType
    const std::string getPlantType();
protected:
    std::string* texturePaths;
    int growthTime; // Time it takes to grow one stage
    std::string plantType; // Type of plant
    int growthStage; // Current growth stage
    unsigned long plantTime; // When plant was planted
};
