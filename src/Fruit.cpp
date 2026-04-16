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
    b2Vec2 position = m_PhysicsBody->GetPosition();
    float angle = m_PhysicsBody->GetAngle();
    // 拿出來畫圖時：把公尺 乘以 PTM 變回像素
    m_VisualComponent->m_Transform.translation = {position.x * PTM, position.y * PTM};
    m_VisualComponent->m_Transform.rotation = angle;
}
void Fruit::Draw() {
    if (m_VisualComponent) {
        m_VisualComponent->Draw();
    }
}