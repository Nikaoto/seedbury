#include "cocos2d.h"
#include "MainScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/document.h"
#include "json/writer.h"
#include <iostream>
#include "json/prettywriter.h"
#include "Land.h"
#include "DBManager.h"
#include "MenuPanel.h"

using namespace cocos2d;

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    log("scene created");
    
    this->menuPanel = nullptr;

    const auto director = Director::getInstance();
    const auto size = director->getVisibleSize();
    const auto origin = director->getVisibleOrigin();
    
    // Background
    director->setClearColor(Color4F::WHITE);
    const auto grassBackground = Sprite::create("grass_background.jpg");
    grassBackground->setPosition(origin.x + size.width/2, origin.y + size.height/2);
    this->addChild(grassBackground, -3);

    // Land grids
    const auto grid_margin = 10;
    const auto margin_vertical = 100;
    const auto margin_horizontal = 200;
    const auto horizontal_land_count = 6;
    const auto vertical_land_count = 3;
    const auto land_count = horizontal_land_count * vertical_land_count;
    // Store each land object
    auto landVector = cocos2d::Vector<Land*>(land_count);
    // Starting position
    auto p = Vec2(origin.x + margin_horizontal, origin.y + size.height - margin_vertical);
    int landCounter = 0;
    // Get all plants from db
    //auto plantMap = dbManager->getPlants();
    // Draw both 3x3 grids
    for (int x = 0; x < horizontal_land_count; x++) {
        for (int y = 0; y < vertical_land_count; y++) {
            auto land = Land::create(landCounter);
            auto s = land->getBoundingBox().size;
            auto pos = Vec2(p.x + s.width * x + grid_margin * x, p.y - s.height * y - grid_margin * y);
            
            // Add distance between the two grids
            if (x > 2) {
                pos.set(pos.x + grid_margin * 6, pos.y);
            }
            
            land->setPosition(pos);
            landVector.pushBack(land);
            this->addChild(land);
            landCounter += 1;
        }
    }
    
    // Stats button
    const auto margin = Vec2(30, 30);
    this->statsButton = ui::Button::create();
    const auto statsScale = 2;
    statsButton->loadTextureNormal("ui/stats_button.png");
    statsButton->setPressedActionEnabled(true);
    statsButton->setZoomScale(0.3);
    statsButton->setScale(statsScale);
    statsButton->setPosition(Vec2(
        origin.x + statsButton->getContentSize().width/2 * statsScale + margin.x,
        origin.y + size.height - statsButton->getContentSize().height/2 * statsScale - margin.y));
    statsButton->setSwallowTouches(true);
    statsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            triggerMenu();
            return true;
        }
        return false;
    });
    this->addChild(statsButton, 2);

    return true;
}

void MainScene::triggerMenu() {
    if (this->menuPanel == nullptr) {
        // Create panel
        const auto director = Director::getInstance();
        const auto size = director->getVisibleSize();
        const auto origin = director->getVisibleOrigin();
        
        CCLOG("building MP");
        this->menuPanel = MenuPanel::Builder(director)
                .setSize(size.width*0.8, size.height*0.8)
                .setPosition(origin.x + size.width * 0.5, origin.y + size.height * 0.5)
                .setText("Lorem ipsum dolor sit amet  test test test test test test test test vtest. test test test test test")
                .setPositiveButton("Yes", [&](){ CCLOG("POS BTN PRESSED"); })
                .setNegativeButton("No", [&]() { CCLOG("NEG BTN PRESSED"); })
                .setBackgroundDim(true)
                .onOutsideClick([&]() {
                    this->triggerMenu();
                })
                .build();
        CCLOG("MP created");
        CCLOG("adding mp as child");
        this->addChild(this->menuPanel, 3);
    } else {
        // Remove panel
        this->removeChild(this->menuPanel);
        this->menuPanel = nullptr;
    }
}
