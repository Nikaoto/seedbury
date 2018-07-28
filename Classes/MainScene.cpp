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

using namespace cocos2d;

cocos2d::Scene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    log("scene created");

    const auto director = Director::getInstance();
    const auto size = director->getVisibleSize();
    const auto origin = director->getVisibleOrigin();

    const auto margin = Vec2(30, 30);

    // Background
    director->setClearColor(Color4F::WHITE);

#pragma region GRID_SETUP
    const auto grid_margin = 10;
    const auto margin_vertical = 100;
    const auto margin_horizontal = 200;
    const auto horizontal_land_count = 6;
    const auto vertical_land_count = 3;
    const auto land_count = horizontal_land_count * vertical_land_count;
    
    auto landVector = cocos2d::Vector<Land*>(land_count);
    
    auto p = Vec2(origin.x + margin_horizontal, origin.y + size.height - margin_vertical);
    
    // Draw both 3x3 grids
    for (int x = 0; x < horizontal_land_count; x++) {
        for (int y = 0; y < vertical_land_count; y++) {
            auto land = Land::create();
            auto s = land->getBoundingBox().size;
            auto pos = Vec2(p.x + s.width * x + grid_margin * x, p.y - s.height * y - grid_margin * y);
            
            // Add distance between the two grids
            if (x > 2) {
                pos.set(pos.x + grid_margin * 6, pos.y);
            }
            
            land->setPosition(pos);
            landVector.pushBack(land);
            this->addChild(land);
        }
    }
#pragma endregion


#pragma region UI_SETUP
    // Light slider
    auto slider = ui::Slider::create();
    const auto sliderScale = 2;
    slider->loadBarTexture("ui/slider_background.png");
    slider->loadProgressBarTexture("ui/slider_foreground.png");
    slider->loadSlidBallTextureNormal("ui/slider_thumb_normal.png");
    //slider->loadSlidBallTextures("ui/slider_thumb_normal.png", "ui/slider_thumb_pressed.png", "ui/slider_thumb_disabled.png");
    slider->setScale(sliderScale);
    slider->setPosition(Vec2(
		origin.x + size.width/2, 
		origin.y + slider->getContentSize().height/2 * sliderScale + margin.y*1.5));
	slider->setZoomScale(0.4);
    slider->setPercent(50);
    this->addChild(slider);
#pragma endregion


    // Set up touch listener
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = [&](Touch* touch, Event* event) {
//        return true;
//    };
    
    //getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
