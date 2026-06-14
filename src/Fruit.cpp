#include "Fruit.hpp"
#include "Util/Image.hpp"
#include <iostream>

const float PTM = 30.0f;

// ==========================================
// 水果類別的實作
// ==========================================
Fruit::Fruit(b2World* world, b2Vec2 position, const std::string& imagePath, float radius, FruitLevel level, float scale)
    : m_Radius(radius), m_Level(level) {
    // === 設定圖片 ===
    m_VisualComponent = std::make_shared<Util::GameObject>();
    m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
    m_VisualComponent->SetZIndex(11);
    m_VisualComponent->m_Transform.scale = {scale, scale};
    // === 設定靈魂 Box2D 動態剛體 ===
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    // 存入物理世界時：把像素座標 除以 PTM 變成公尺
    bodyDef.position.Set(position.x / PTM, position.y / PTM);
    m_PhysicsBody = world->CreateBody(&bodyDef);
    // 給剛體掛上身分證：把「這顆水果物件自己(this)」的指標存進去
    m_PhysicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
    // === 設定碰撞形狀與材質 ===
    b2CircleShape dynamicCircle;
    // 半徑也要：把像素 除以 PTM 變成公尺
    dynamicCircle.m_radius = radius / PTM;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    fixtureDef.density = 1.0f + (static_cast<float>(level) * 0.5f);
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.15f;

    m_PhysicsBody->CreateFixture(&fixtureDef);
}
void Fruit::Update() {
}
void Fruit::Draw() {
    if (!m_VisualComponent) return; // 也要改成 m_VisualComponent

    // 取得 Box2D 的真實物理座標
    b2Vec2 pos = m_PhysicsBody->GetPosition();
    float visualX = pos.x * PTM;
    float visualY = pos.y * PTM;

    // ✨ 如果是在哭泣狀態，就產生一個極小的隨機偏移來製造抖動 (X 跟 Y 隨機 -3.0 ~ 3.0)
    if (m_IsSad) {
        visualX += ((std::rand() % 7) - 3) * 1.0f;
        visualY += ((std::rand() % 7) - 3) * 1.0f;
    }

    // 將算好的視覺座標套用到 GameObject 上
    m_VisualComponent->m_Transform.translation = {visualX, visualY};
    m_VisualComponent->m_Transform.rotation = m_PhysicsBody->GetAngle();

    m_VisualComponent->Draw();
    }
void Fruit::SetSadStatus(bool isSad) {
    m_IsSad = isSad;
    std::string path = "Resources/material/fruit/";

    // 如果是 isSad 就加上 "_sad.png"，否則就是普通的 ".png"
    std::string suffix = isSad ? "_sad.png" : ".png";

    switch (GetLevel()) {
        case FruitLevel::Cherry: path += "cherry" + suffix; break;
        case FruitLevel::Strawberry: path += "strawberry" + suffix; break;
        case FruitLevel::Grape: path += "grape" + suffix; break;
        case FruitLevel::tangerine: path += "tangerine" + suffix; break;
        case FruitLevel::Orange: path += "orange" + suffix; break;
        case FruitLevel::Apple: path += "apple" + suffix; break;
        case FruitLevel::Pear: path += "pear" + suffix; break;
        case FruitLevel::Peach: path += "peach" + suffix; break;
        case FruitLevel::Pineapple: path += "pineapple" + suffix; break;
        case FruitLevel::Melon: path += "melon" + suffix; break;
        case FruitLevel::Watermelon: path += "watermelon" + suffix; break;
        default: path += "cherry" + suffix; break;
    }

    m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(path));
}