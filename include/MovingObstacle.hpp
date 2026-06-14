#ifndef MOVINGOBSTACLE_HPP
#define MOVINGOBSTACLE_HPP

#include "Obstacle.hpp"
#include "Util/Image.hpp"
#include <string>

class MovingObstacle : public Obstacle {
private:
    const float PTM = 30.0f;
    float m_LeftBound, m_RightBound;
    float m_Speed;

public:
    MovingObstacle(b2World* world, b2Vec2 position, float leftBound, float rightBound, float speed, const std::string& imagePath, float radius, float scale)
        : m_LeftBound(leftBound / PTM), m_RightBound(rightBound / PTM), m_Speed(speed) {
        
        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5);
        m_VisualComponent->m_Transform.scale = {scale, scale};
        m_VisualComponent->m_Transform.translation = {position.x * PTM, position.y * PTM};

        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody; // 運動剛體：不受重力影響，但會推動動態剛體
        bodyDef.position.Set(position.x, position.y);
        m_PhysicsBody = world->CreateBody(&bodyDef);

        b2CircleShape circleShape;
        circleShape.m_radius = radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 10.0f;
        m_PhysicsBody->CreateFixture(&fixtureDef);
        
        m_PhysicsBody->SetLinearVelocity(b2Vec2(m_Speed, 0.0f)); // 初始速度
    }

    void Update() override {
        b2Vec2 pos = m_PhysicsBody->GetPosition();
        b2Vec2 vel = m_PhysicsBody->GetLinearVelocity();

        // 碰到邊界反向
        if (pos.x >= m_RightBound && vel.x > 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(-m_Speed, 0.0f));
        else if (pos.x <= m_LeftBound && vel.x < 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(m_Speed, 0.0f));

        m_VisualComponent->m_Transform.translation = {pos.x * PTM, pos.y * PTM};
    }
};
#endif