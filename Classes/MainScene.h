#pragma once
#include "cocos2d.h"
#include "Land.h"
#include "ui/CocosGUI.h"
#include "MenuPanel.h"

class MainScene : public cocos2d::Scene {
public:
    bool init() override;
    CREATE_FUNC(MainScene);
    
    void triggerMenu();
    void triggerPlantMenu(int senderLandNumber);
protected:
    cocos2d::Vector<Land*> landVector;
    Land* selectedLand;
    cocos2d::ui::ScrollView* scrollView;
    cocos2d::ui::Button* statsButton;
    MenuPanel* menuPanel;
};
