#include "MenuPanel.h"
#include "cocos2d.h"
#include <functional>
#include "ui/CocosGUI.h"

// Constants
const cocos2d::Vec2 MenuPanel::TEXT_MARGIN = cocos2d::Vec2(30, 30);
const cocos2d::Vec2 MenuPanel::BUTTON_MARGIN = cocos2d::Vec2(30, 30);
const cocos2d::Size MenuPanel::BUTTON_SIZE = cocos2d::Size(250, 150);

MenuPanel* MenuPanel::create(MenuPanel::MenuProps& props) {
    MenuPanel *pRet = new(std::nothrow) MenuPanel(props);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

MenuPanel::MenuPanel(MenuProps& props) {
    this->props = props;
}

bool MenuPanel::init() {
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    
    this->active = false;
    
    const auto size = props.director->getVisibleSize();
    
    /* Setting props */
    
    // Panel position
    this->setPosition(props.position);
    
    // Background dim
    if (props.backgroundDim) {
        const auto dimColor = cocos2d::Color4F(0, 0, 0, 127);
        const auto drawNode = cocos2d::DrawNode::create();
        const cocos2d::Vec2 vertices[4] = {
            cocos2d::Vec2(0, 0),
            cocos2d::Vec2(size.width, 0),
            cocos2d::Vec2(size.width, size.height),
            cocos2d::Vec2(0, size.height)
        };
        drawNode->drawPolygon(vertices, 4, dimColor, 0, dimColor);
        const auto ap = this->getAnchorPoint();
        drawNode->setPosition(
            props.panelSize.width * ap.x - props.position.x,
            props.panelSize.height * ap.y - props.position.y);
        this->addChild(drawNode, -1);
    }
    
    // Tile texture
    const auto tex = props.director->getTextureCache()->addImage(props.texturePath);
    tex->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
    const auto rect = cocos2d::Rect(0, 0, props.panelSize.width, props.panelSize.height);
    this->initWithTexture(tex, rect);
    
    // Text
    // Use custom ttf config if specified
    if (props.ttfConfig != nullptr) {
        this->textLabel = cocos2d::Label::createWithTTF((*props.ttfConfig), props.text);
    } else {
        // Use default ttf config
        auto ttfConfig = cocos2d::TTFConfig();
        ttfConfig.outlineSize = 2;
        ttfConfig.fontFilePath = "fonts/arial.ttf";
        ttfConfig.fontSize = 34;
        this->textLabel = cocos2d::Label::createWithTTF(ttfConfig, props.text);
    }
    this->textLabel->enableOutline(cocos2d::Color4B::BLACK);
    this->textLabel->setMaxLineWidth(props.panelSize.width - TEXT_MARGIN.x * 2);
    // Text position
    const auto labelSize = this->textLabel->getContentSize();
    this->textLabel->setPosition(
        TEXT_MARGIN.x + labelSize.width/2,
        props.panelSize.height - TEXT_MARGIN.y - labelSize.height/2);
    this->addChild(textLabel, 1);
    
    // Outside click callback
    const auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (this->getBoundingBox().containsPoint(touch->getLocation())) {
            // Tapped inside menu
            return true;
        } else if (props.outsideClickCallback != nullptr) {
            props.outsideClickCallback();
            return true;
        }
        return false;
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // Positive button
    if (props.positiveButton != nullptr) {
        // Adjust position
        const auto bottomLeft = cocos2d::Vec2(0, 0);
        props.positiveButton->setAnchorPoint(cocos2d::Vec2(0, 0));
        props.positiveButton->setPosition(cocos2d::Vec2(bottomLeft.x + BUTTON_MARGIN.x, bottomLeft.y + BUTTON_MARGIN.y));
        // Setup click callback
        props.positiveButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
                props.positiveButtonCallback();
            }
        });
        this->addChild(props.positiveButton, 1);
    }
    
    // Negative button
    if (props.negativeButton != nullptr) {
        // Adjust position
        const auto bottomRight = cocos2d::Vec2(props.panelSize.width, 0);
        props.negativeButton->setAnchorPoint(cocos2d::Vec2(1, 0));
        props.negativeButton->setPosition(cocos2d::Vec2(bottomRight.x - BUTTON_MARGIN.x, bottomRight.y + BUTTON_MARGIN.y));
        // Setup click callback
        props.negativeButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
            if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
                props.negativeButtonCallback();
            }
        });
        this->addChild(props.negativeButton, 1);
    }

    return true;
}

const bool MenuPanel::isActive() {
    return this->active;
}

void MenuPanel::setActive(const bool active) {
    this->active = active;
}

//========================//
/*   MenuPanel Builder   */
//========================//
// Builder constructor
MenuPanel::Builder::Builder(cocos2d::Director* director) {
    this->props.director = director;
    // Set defaults
    this->props.backgroundDim = false;
    this->props.text = "";
    this->props.ttfConfig = nullptr;
    this->props.texturePath = "menu_texture.jpg";
    this->props.position = cocos2d::Vec2(0, 0);
    this->props.outsideClickCallback = nullptr;
    this->props.positiveButton = nullptr;
    this->props.positiveButtonCallback = nullptr;
    this->props.negativeButton = nullptr;
    this->props.negativeButtonCallback = nullptr;
}

MenuPanel::Builder& MenuPanel::Builder::setText(std::string text) {
    this->props.text = text;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setTTFConfig(cocos2d::TTFConfig* ttfConfig) {
    this->props.ttfConfig = ttfConfig;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setTexturePath(std::string texturePath) {
    this->props.texturePath = texturePath;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setBackgroundDim(const bool backgroundDim) {
    this->props.backgroundDim = backgroundDim;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setSize(const float width, const float height) {
    this->props.panelSize = cocos2d::Size(width, height);
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setPosition(const float x, const float y) {
    this->props.position = cocos2d::Vec2(x, y);
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::onOutsideClick(std::function<void()> callback) {
    this->props.outsideClickCallback = callback;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setPositiveButton(std::string text, std::function<void()> callback) {
    auto btn = cocos2d::ui::Button::create("ui/button_normal.png", "ui/button_pressed.png");
    btn->setTitleText(text);
    btn->setTitleColor(cocos2d::Color3B::BLACK);
    const auto s = btn->getContentSize();
    btn->setScale(BUTTON_SIZE.width / s.width, BUTTON_SIZE.height / s.height);
    this->props.positiveButtonCallback = callback;
    this->props.positiveButton = btn;
    return *this;
}

MenuPanel::Builder& MenuPanel::Builder::setNegativeButton(std::string text, std::function<void()> callback) {
    auto btn = cocos2d::ui::Button::create("ui/button_normal.png", "ui/button_pressed.png");
    btn->setTitleText(text);
    btn->setTitleColor(cocos2d::Color3B::BLACK);
    const auto s = btn->getContentSize();
    btn->setScale(BUTTON_SIZE.width / s.width, BUTTON_SIZE.height / s.height);
    this->props.negativeButtonCallback = callback;
    this->props.negativeButton = btn;
    return *this;
}

MenuPanel* MenuPanel::Builder::build() {
    return MenuPanel::create(props);
}
