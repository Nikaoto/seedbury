# seedbury
Follow cli reference 
[here](http://www.cocos2d-x.org/docs/cocos2d-x/en/editors_and_tools/cocosCLTool.html) for building 
and compilation instructions.

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
---
```
// Test internet
if (InternetStatus::isConnected()) {
    this->testHttpImageRequest(Vec2(size.width/2, size.height/2));
} else {
    InternetStatus::toastMessage("Please connect internet");
}
```
