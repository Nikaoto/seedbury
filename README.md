# seedbury
Follow cli reference 
[here](http://www.cocos2d-x.org/docs/cocos2d-x/en/editors_and_tools/cocosCLTool.html) for building 
and compilation instructions.

# TODO
- [ ] Add fertility table to db
- [ ] Plant menu
  - [ ] Add cancel (x) button top right, float  / outside click
    - [x] Add placeholder w/ full functionality
  - [ ] Find/create design for scrollview
  - [x] Create plant class
    - [ ] Add description std::string
    - [x] Add sprout time
- [ ] Add green plant placeholder + BroccoliSproutPlant class

- [x] Add plant reap notification for ios 
  - [ ] Test on device
- [ ] Add plant reap notification for android
  - [ ] Test on device

**Must-have Features**
- [ ] Local notification for water replacement (every 2 weeks)
- [ ] In-app (custom) alert for fertilizer replacement (after reaping 9th plant for a side)
- [ ] Local notification for light switch (every 12 hrs)
- [ ] Local notification when a plant is ready to reap
- [ ] Link to online shop

# Notes
## UI touch listener
### With lambda
```
statsButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        triggerMenu();
    }
});
```
### With CC_CALLBACK_2
```
statsButton->addTouchEventListener(CC_CALLBACK_2(MainScene::triggerMenu, this));

void MainScene::triggerMenu(Ref* pSender, Widget::TouchEventType type)
{
  if (type == ui::Widget::TouchEventType::ENDED) {
      CCLOG("ayy")
  }
}
```

## UI
```
#pragma region UI_SETUP
    // Light slider
    const auto margin = Vec2(30, 30);
    auto slider = ui::Slider::create();
    const auto sliderScale = 2;
    slider->loadBarTexture("ui/slider_background.png");
    slider->loadProgressBarTexture("ui/slider_foreground.png");
    slider->loadSlidBallTextureNormal("ui/slider_thumb_normal.png");
    //slider->loadSlidBallTextures("ui/slider_thumb_normal.png", 
    "ui/slider_thumb_pressed.png", "ui/slider_thumb_disabled.png");
    slider->setScale(sliderScale);
    slider->setPosition(Vec2(
    origin.x + size.width/2, 
    origin.y + slider->getContentSize().height/2 * sliderScale + margin.y*1.5));
    slider->setZoomScale(0.4);
    slider->setPercent(50);
    this->addChild(slider);
    
    // Share button
    auto shareButton = ui::Button::create();
    const auto shareScale = 2;
    shareButton->loadTextureNormal("ui/share_button.png");
    shareButton->setPressedActionEnabled(true);
    shareButton->setZoomScale(0.3);
    shareButton->setScale(shareScale);
    shareButton->setPosition(Vec2(
    origin.x + size.width - shareButton->getContentSize().width/2 * shareScale - margin.x,
    origin.y + shareButton->getContentSize().height/2 * shareScale + margin.y));
    this->addChild(shareButton);
    
    // Stats button
    auto statsButton = ui::Button::create();
    const auto statsScale = 2;
    statsButton->loadTextureNormal("ui/stats_button.png");
    statsButton->setPressedActionEnabled(true);
    statsButton->setZoomScale(0.3);
    statsButton->setScale(statsScale);
    statsButton->setPosition(Vec2(
    origin.x + statsButton->getContentSize().width/2 * statsScale + margin.x,
    origin.y + size.height - statsButton->getContentSize().height/2 * statsScale - margin.y));
    this->addChild(statsButton);
    
    // Fertilizer button
    auto fertilizerButton = ui::Button::create();
    const auto fertilizerScale = 2;
    const auto fertilizerMarginTop = margin.y * 2;
    fertilizerButton->loadTextureNormal("ui/fertilizer_button.png");
    fertilizerButton->setPressedActionEnabled(true);
    fertilizerButton->setZoomScale(0.3);
    fertilizerButton->setScale(fertilizerScale);
    fertilizerButton->setPosition(Vec2(
    origin.x + fertilizerButton->getContentSize().width/2 * fertilizerScale + margin.x,
    statsButton->getPositionY() - statsButton->getContentSize().height/2 * statsScale 
    - fertilizerButton->getContentSize().height/2 * fertilizerScale - fertilizerMarginTop));
    this->addChild(fertilizerButton);
    
    // Water button
    auto waterButton = ui::Button::create();
    const auto waterScale = 2;
    const auto waterMarginTop = margin.y * 2;
    waterButton->loadTextureNormal("ui/water_button.png");
    waterButton->setPressedActionEnabled(true);
    waterButton->setZoomScale(0.3);
    waterButton->setScale(waterScale);
    waterButton->setPosition(Vec2(
    origin.x + waterButton->getContentSize().width/2 * waterScale + margin.x,
    fertilizerButton->getPositionY() - fertilizerButton->getContentSize().height/2 * fertilizerScale 
    - waterButton->getContentSize().height/2 * waterScale - waterMarginTop));
    this->addChild(waterButton);
#pragma endregion
```
---

## Time api
`https://www.worldtimeserver.com/handlers/GetData.ashx?action=GCTData&_=1532888676507`
returns:
```
{
  LocIDDescription: "Georgia - ",
  LocIDLocationID: "GE",
  CountryCode: "GE",
  CountryName: "Georgia",
  State: "",
  City: "Unknown",
  TimeZone: "Asia/Tbilisi",
  DstZoneName: "",
  DstZoneAbbrev: "",
  StdZoneName: "Georgia Standard Time",
  StdZoneAbbrev: "GET",
  IsInDST: false,
  ThisTime: "2018-07-29T22:26:03.0614369Z",
  DateTime_12HR: "10:26 PM",
  DateTime_24HR: "22:26:03",
  FormattedDate: "Sunday, July 29, 2018",
  Lat: 42,
  Lng: 43.5,
  ErrorMsg: null,
  serverTimeStamp: 1532903163061.4368,
  dataFrom: "IP"
}
```

## Test internet connection
```
void MainScene::testHttpRequest() {
    auto request = new (std::nothrow) network::HttpRequest();
    request->setUrl("https://jsonplaceholder.typicode.com/users/1");
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback([&](network::HttpClient* sender, network::HttpResponse* response) {
        log("http response received");
        if (!response) {
            return;
        }

        // Parse json
        const std::vector<char>* res = response->getResponseData();
        std::string str  = res->data();

        // Replace random garble at the end of json string with nothing
        std::size_t pos = str.rfind('}');
        if (pos != std::string::npos) {
            pos += 1;
            str.replace(pos, str.length() - pos, "");
        }
        //

        // Print pretty json with rapidjson
        rapidjson::Document doc;
        doc.Parse(str.c_str());
        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        doc.Accept(writer);
        log("%s", sb.GetString());
    });

    network::HttpClient::getInstance()->sendImmediate(request);
    log("http request sent");
    request->release();
}

void MainScene::testHttpImageRequest(Vec2 position) {
    auto request = new (std::nothrow) network::HttpRequest();
    request->setUrl("https://http.cat/100.jpg");
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback([&](network::HttpClient* sender, network::HttpResponse* response) {
        log("image response received");
        if (!response) {
            return;
        }

        auto buffer = response->getResponseData();
        auto image = new Image();
        image->initWithImageData(reinterpret_cast<const unsigned char*>(&(buffer->front())), buffer->size());

        auto texture = new Texture2D();
        texture->initWithImage(image);
        //auto sprite = Sprite::create("cocos.png");
        auto sprite = Sprite::createWithTexture(texture);
        sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        const auto size = Director::getInstance()->getVisibleSize();
        const auto origin = Director::getInstance()->getVisibleOrigin();
        sprite->setPosition(origin.x + size.width/2, origin.y + size.height/2);

        this->addChild(sprite, -2);
    });

    network::HttpClient::getInstance()->sendImmediate(request);
    log("image request sent");
    request->release();
}
```
```
// Test internet
if (InternetStatus::isConnected()) {
    this->testHttpImageRequest(Vec2(size.width/2, size.height/2));
} else {
    InternetStatus::toastMessage("Please connect internet");
}
```

## Tile texture on 2D plane (rect)
```
// Tile texture
const auto tex = Director::getInstance()->getTextureCache()->addImage("path/to/tex.jpg");
tex->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
const auto rect = cocos2d::Rect(0, 0, rectSize.width, rectSize.height);
this->initWithTexture(tex, rect);
```
---
