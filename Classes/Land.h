#pragma once
#include "cocos2d.h"

const static cocos2d::Size LAND_SIZE = cocos2d::Size(200, 200);

// Sprite locations
const static std::string FERTILE_SPRITE = "land_fertile.png";
const static std::string INFERTILE_SPRITE = "land_infertile.png";

class Land : public cocos2d::Node {
public:
    bool init() override;

    cocos2d::DrawNode* drawNode;
    
    void setFertile(bool fertile);
    bool isFertile();
    
    CREATE_FUNC(Land);
protected:
    bool fertile;
};
