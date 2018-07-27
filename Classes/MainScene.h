#pragma once
#include "cocos2d.h"
#include "Land.h"

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(MainScene);
    bool init() override;

protected:
    cocos2d::Vector<Land*> landVector;
    //cocos2d::EventListenerTouchOneByOne listener;
};
