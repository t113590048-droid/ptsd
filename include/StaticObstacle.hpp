#ifndef STATICOBSTACLE_HPP
#define STATICOBSTACLE_HPP

#include "Obstacle.hpp"
#include "Util/Image.hpp"
#include <string>

class StaticObstacle : public Obstacle {
public:
    // PTM 轉換比例 (配合你專案的 30.0f)
    const float PTM = 30.0f;

    StaticObstacle(b2World* world, b2Vec2 position, const std::string& imagePath, float radius, float scale = 1.0f) {
        // === 1. 設定視覺圖片 ===
        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5); // 畫在背景前，水果後
        m_VisualComponent->m_Transform.scale = {scale, scale};
        // Box2D 的公尺轉回像素畫圖
        m_VisualComponent->m_Transform.translation = {position.x * PTM, position.y * PTM};

        // === 2. 設定 Box2D 物理剛體 (靜態物件) ===
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody; // ★ 關鍵：靜態剛體，懸浮在空中不會掉落，也不會被撞飛
        bodyDef.position.Set(position.x, position.y);
        m_PhysicsBody = world->CreateBody(&bodyDef);

        // === 3. 設定碰撞形狀與材質 ===
        b2CircleShape circleShape;
        circleShape.m_radius = radius; // 公尺

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 0.0f;     // 靜態物件密度通常設為 0
        fixtureDef.friction = 0.3f;    // 摩擦力
        fixtureDef.restitution = 0.3f; // 彈力 (0.3 撞到會稍微彈一下)

        m_PhysicsBody->CreateFixture(&fixtureDef);
    }

    void Update() override {
        // 靜態釘柱不動如山，不需要 Update 邏輯
    }
};

#endif // STATICOBSTACLE_HPP