#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MenuPanel : public cocos2d::Sprite {
public:
    // Constants
    static const cocos2d::Vec2 TEXT_MARGIN;
    static const cocos2d::Vec2 BUTTON_MARGIN;
    static const cocos2d::Size BUTTON_SIZE;
private:
    class MenuProps {
        std::string text;
        cocos2d::TTFConfig* ttfConfig;
        bool backgroundDim;
        cocos2d::Size panelSize;
        std::string texturePath; // Used to load texture at build()
        cocos2d::Vec2 position;
        cocos2d::Director* director;
        std::function<void()> outsideClickCallback;
        cocos2d::ui::Button* positiveButton;
        std::function<void()> positiveButtonCallback;
        cocos2d::ui::Button* negativeButton;
        std::function<void()> negativeButtonCallback;
        
        friend class MenuPanel;
        friend class Builder;
    };
    
    // Initialization
    static MenuPanel* create(MenuProps& props);
    MenuPanel(MenuProps& props);
    bool init() override;
    
    // Props set with builder
    MenuProps props;
    
    // Other props
    cocos2d::Label* textLabel;
    bool active;
public:
    class Builder {
    private:
        MenuProps props;
        friend class MenuPanel::MenuProps;
    public:
        Builder(cocos2d::Director* director);
        Builder& setText(std::string text);
        Builder& setTTFConfig(cocos2d::TTFConfig* ttfConfig);
        Builder& setBackgroundDim(const bool backgroundDim);
        Builder& setSize(const float width, const float height);
        Builder& setPosition(const float x, const float y);
        Builder& setTexturePath(std::string texturePath);
        Builder& onOutsideClick(std::function<void()> callback);
        Builder& setPositiveButton(std::string text, std::function<void()> callback);
        Builder& setNegativeButton(std::string text, std::function<void()> callback);
        MenuPanel* build();
    };
    
    const bool isActive();
    void setActive(const bool active);
};

