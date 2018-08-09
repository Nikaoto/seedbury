#include "MenuPanel.h"
#include "cocos2d.h"
#include <functional>

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
    // Position
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
        CCLOG("ap: %f, %f", ap.x, ap.y);
        drawNode->setPosition(
            props.panelSize.width * ap.x - props.position.x,
            props.panelSize.height * ap.y - props.position.y);
        //props.position.x - props.panelSize.width * ap.x, props.position.y - props.panelSize.height * ap.y);
        this->addChild(drawNode, -1);
    }
    // Tile texture
    const auto tex = props.director->getTextureCache()->addImage(props.texturePath);
    tex->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
    const auto rect = cocos2d::Rect(0, 0, props.panelSize.width, props.panelSize.height);
    this->initWithTexture(tex, rect);
    // Text
    this->textLabel = cocos2d::Label::createWithTTF(props.text, "fonts/arial.ttf", 32);
    const auto labelSize = this->textLabel->getContentSize();
    const auto margin = cocos2d::Vec2(30, -30);
    this->textLabel->setPosition(margin.x + labelSize.width/2, margin.y + props.panelSize.height);
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
    this->props.texturePath = "menu_texture.jpg";
    this->props.position = cocos2d::Vec2(0, 0);
    this->props.outsideClickCallback = nullptr;
}

MenuPanel::Builder& MenuPanel::Builder::setText(std::string text) {
    this->props.text = text;
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

MenuPanel* MenuPanel::Builder::build() {
    return MenuPanel::create(props);
}
