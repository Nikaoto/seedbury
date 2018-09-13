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
#define len(arr) sizeof(arr) / sizeof(arr[0])

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
    this->landVector = cocos2d::Vector<Land*>(land_count);
    // Starting position
    auto p = Vec2(origin.x + margin_horizontal, origin.y + size.height - margin_vertical);
    int landCounter = 0;
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
            land->setPlantMenuCallback([&](int senderLandNumber){
                this->triggerPlantMenu(senderLandNumber);
            });
            this->landVector.pushBack(land);
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
        // Collect screen vars
        const auto director = Director::getInstance();
        const auto size = director->getVisibleSize();
        const auto origin = director->getVisibleOrigin();
        // Create panel
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
        this->addChild(this->menuPanel, 3);
    } else {
        // Remove panel
        this->removeChild(this->menuPanel);
        this->menuPanel = nullptr;
    }
}

void MainScene::triggerPlantMenu(int senderLandNumber) {
    // Set selected land
    this->selectedLand = this->landVector.at(senderLandNumber);

    const auto plantCount = len(Plant::PLANT_TYPES);
    // Pop up plant menu
    const auto s = Director::getInstance()->getVisibleSize();
    const auto o = Director::getInstance()->getVisibleOrigin();
    const auto scrollViewMargin = 30;
    const auto plantButtonSize = Size(160, 160);
    const auto plantButtonMargin = Vec2(80, -20);
    const auto plantTitleMargin = 70;
    this->scrollView = ui::ScrollView::create();
    scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setContentSize(Size(s.width, s.height));
    scrollView->setInnerContainerSize(Size(
        scrollViewMargin * 2 + plantCount * (plantButtonSize.width + plantButtonMargin.x) + plantButtonMargin.x,
        s.height));
    scrollView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    scrollView->setBackGroundColor(Color3B::BLACK);
    scrollView->setBackGroundColorOpacity(200);
    scrollView->setSwallowTouches(true);
    scrollView->setBounceEnabled(true);
    scrollView->setAnchorPoint(Vec2(0.5, 0.5));
    scrollView->setPosition(Vec2(o.x + s.width/2, o.y + s.height/2));

    // Populate with plant buttons
    unsigned int i = 1;
    for (const auto& element : Plant::PLANT_DATA) {
        // Create plant button
        auto b = ui::Button::create(element.second.texturePaths[3]);
        b->setAnchorPoint(Vec2(0.5, 0.5));
        b->setScale(plantButtonSize.width / b->getContentSize().width, plantButtonSize.height / b->getContentSize().height);
        const auto buttonPosition = Vec2(
            i * (plantButtonSize.width + plantButtonMargin.x),
            scrollView->getContentSize().height - scrollViewMargin - b->getContentSize().height / 2 + + plantButtonMargin.y);
        b->setPosition(buttonPosition);
        // Click listener
        b->addClickEventListener([&](Ref* target) {
            this->selectedLand->plantPlant(element.second.type);
            this->scrollView->removeFromParent();
            this->scrollView = nullptr;
            this->selectedLand = nullptr;
            // Remove cancel button
            if (this->scrollViewCancelButton != nullptr) {
                this->scrollViewCancelButton->removeFromParent();
                this->scrollViewCancelButton = nullptr;
            }
        });
        // Add wooden frame
        auto plantFrame = Sprite::create("ui/wooden_frame.png");
        const auto scaleMargin = Size(1, 1.2);
        plantFrame->setScale(b->getContentSize().width / plantFrame->getContentSize().width * scaleMargin.width,
            b->getContentSize().height / plantFrame->getContentSize().height * scaleMargin.height);
        plantFrame->setAnchorPoint(Vec2(0.5, 0.5));
        plantFrame->setPosition(Vec2(b->getContentSize().width/2, b->getContentSize().height/2));
        b->addChild(plantFrame, -1);
        // Add plant title text
        auto plantTitle = Label::createWithSystemFont(element.second.type, "Arial", 30);
        plantTitle->setAnchorPoint(Vec2(0.5, 0));
        plantTitle->setPosition(b->getContentSize().width / 2, b->getContentSize().height + plantTitleMargin);
        plantTitle->setScale(1 / b->getScaleX(), 1 / b->getScaleY());
        b->addChild(plantTitle);
        //
        scrollView->addChild(b);
        i++;
    }
    
    // Add cancel button
    this->scrollViewCancelButton = ui::Button::create("ui/cancel_button.png");
    auto scale = MenuPanel::CANCEL_BUTTON_SIZE.width / scrollViewCancelButton->getContentSize().width;
    auto margin = Vec2(20, 20);
    scrollViewCancelButton->setPressedActionEnabled(true);
    scrollViewCancelButton->setZoomScale(0.3);
    scrollViewCancelButton->setScale(scale);
    scrollViewCancelButton->setPosition(Vec2(
        o.x + s.width - scrollViewCancelButton->getContentSize().width/2 * scale - margin.x,
        o.y + s.height - scrollViewCancelButton->getContentSize().height/2 * scale - margin.y));
    scrollViewCancelButton->addClickEventListener([&](Ref* sender) {
        // Remove scollView
        if (this->scrollView != nullptr) {
            this->scrollView->removeFromParent();
            this->scrollView = nullptr;
        }
        // Remove self (cancel button)
        this->scrollViewCancelButton->removeFromParent();
        this->scrollViewCancelButton = nullptr;
    });
    scrollViewCancelButton->setSwallowTouches(true);
    this->addChild(scrollView, 4);
    this->addChild(scrollViewCancelButton, 4);
}
