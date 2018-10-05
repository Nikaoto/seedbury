#pragma once
#include "cocos2d.h"
#include <iostream>

struct PlantData {
    std::string type;
    int growthTime;
    std::string description;
    std::string* texturePaths;
    
    PlantData(std::string type, int growthTime, std::string description, std::string* texturePaths) {
        this->type = type;
        this->growthTime = growthTime;
        this->description = description;
        this->texturePaths = texturePaths;
    };
    
    PlantData(std::string type, int growthTime) {
        this->type = type;
        this->growthTime = growthTime;
        
        // Default sample description
        this->description = "Lorem Ipsum is simply dummy text of the printing and typesetting blah bluh industry";
        
        // Auto set texture paths based on type
        const auto d = "plants/";
        auto t = std::string(type);
        /// Plant name to lowercase
        for (int i = 0; i < t.length(); i++) {
            t[i] = tolower(t[i]);
        }
        this->texturePaths = new std::string [4] {
            std::string(d).append("standard_0.png"),
            std::string(d).append(t).append("_1.png"),
            std::string(d).append(t).append("_2.png"),
            std::string(d).append(t).append("_3.png")
        };
    };
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
    /// plantType
    const std::string getPlantType();
    /// growthTime
    const int getGrowthTime();
protected:
    std::string* texturePaths;
    int growthTime; // Time it takes to grow one stage in seconds
    std::string plantType; // Type of plant
    int growthStage; // Current growth stage
    unsigned long plantTime; // When plant was planted
};
