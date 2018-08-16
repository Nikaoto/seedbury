#pragma once
#include "cocos2d.h"
#include "Plant.h"

class Plant;

class Land : public cocos2d::Sprite {
public:
    // Constants
    static const cocos2d::Size SIZE;
    static const std::string FERTILE_SPRITE;
    static const std::string INFERTILE_SPRITE;

    // Init
    static Land* create(int landNumber);
    Land(int landNumber);
    bool init() override;

    // Functions
    void plantPlant(std::string plantType);
    void harvestPlant();

    // Getters and setters
    /// landNumber
    void setLandNumber(const int landNumber);
    const int getLandNumber();
    /// fertile
    void setFertile(bool fertile);
    const bool isFertile();
    /// plant
    void setPlant(Plant* plant);
    const Plant* getPlant();
    /// plantMenuCallback
    void setPlantMenuCallback(std::function<void(int senderLandNumber)> callback);
protected:
    int landNumber;
    bool fertile;
    Plant* plant;
    std::function<void(int senderLandNumber)> plantMenuCallback;
};
