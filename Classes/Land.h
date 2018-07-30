#pragma once
#include "cocos2d.h"
#include "Plant.h"

const static cocos2d::Size LAND_SIZE = cocos2d::Size(128, 128);

// Sprite paths
const static std::string FERTILE_SPRITE = "land_fertile.png";
const static std::string INFERTILE_SPRITE = "land_infertile.png";
//
class Plant;

class Land : public cocos2d::Sprite {
public:
    bool init() override;
    
    Plant* plant;
    
    void setFertile(bool fertile);
    const bool isFertile();
    
    void setLandNumber(int landNumber);
    const int getLandNumber();
    
    CREATE_FUNC(Land);
protected:
    bool fertile;
    int landNumber;
};
