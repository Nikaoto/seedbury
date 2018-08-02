#include "AppDelegate.h"
#include "MainScene.h"
#include "HelloWorldScene.h"

#define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

using namespace cocos2d;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2560, 1440);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
    #if USE_AUDIO_ENGINE
    AudioEngine::end();
    #elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
    #endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs() {
    // Set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// Don't modify if using packages obtained through package manager
static int register_all_packages() {
    return 0; // Flag for package manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect(
            "seedbury", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        #else
        glview = GLViewImpl::create("seedbury");
        #endif
        director->setOpenGLView(glview);
    }
    // FPS display
    director->setDisplayStats(true);
    // Set FPS; default is 1/60
    director->setAnimationInterval(1.0f / 60);

    // Set design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    const auto frameSize = glview->getFrameSize();
    // Set content scale factor depending on frame size
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    } else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    } else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    // Create MainScene
    auto scene = MainScene::createScene();
    // Run
    director->runWithScene(scene);

    return true;
}

// Called when app becomes inactive
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    #if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
    #elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    #endif
}

// Called when the app becomes active
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    #if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
    #elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
    #endif
}
