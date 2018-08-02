#pragma once
#include "cocos2d.h"
#include "Land.h"
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Scene {
public:
    bool init() override;
    CREATE_FUNC(MainScene);
    
    void triggerMenu();

protected:
    cocos2d::Vector<Land*> landVector;
    cocos2d::ui::Button* statsButton;
};
