#pragma once
#include "cocos2d.h"

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(MainScene);
    bool init() override;

    void testHttpRequest();
    void testHttpImageRequest(cocos2d::Vec2 position);
};