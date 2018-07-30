#pragma once
#include "cocos2d.h"
#include "Plant.h"

class Plant;

class Land : public cocos2d::Sprite {
public:
    static const cocos2d::Size SIZE;
    static const std::string FERTILE_SPRITE;
    static const std::string INFERTILE_SPRITE;

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
