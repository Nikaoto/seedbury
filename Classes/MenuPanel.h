#pragma once
#include "cocos2d.h"

class MenuPanel : public cocos2d::Sprite {
private:
    class MenuProps {
        std::string text;
        bool backgroundDim;
        cocos2d::Size panelSize;
        std::string texturePath; // Used to load texture at build()
        cocos2d::Vec2 position;
        cocos2d::Director* director;
        std::function<void()> outsideClickCallback;
        
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
        Builder& setBackgroundDim(const bool backgroundDim);
        Builder& setSize(const float width, const float height);
        Builder& setPosition(const float x, const float y);
        Builder& setTexturePath(std::string texturePath);
        Builder& onOutsideClick(std::function<void()> callback);
        MenuPanel* build();
    };
    
    const bool isActive();
    void setActive(const bool active);
};

