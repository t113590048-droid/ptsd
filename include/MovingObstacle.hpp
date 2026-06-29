#ifndef MOVINGOBSTACLE_HPP
#define MOVINGOBSTACLE_HPP

#include "Obstacle.hpp"
#include "Util/Image.hpp"
#include <string>
#include <cmath>

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
        if (pos.x >= m_RightBound && vel.x > 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(-std::abs(m_Speed), 0.0f));
        else if (pos.x <= m_LeftBound && vel.x < 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(std::abs(m_Speed), 0.0f));

        m_VisualComponent->m_Transform.translation = {pos.x * PTM, pos.y * PTM};
    }
};

class VerticalMovingObstacle : public Obstacle {
private:
    const float PTM = 30.0f;
    float m_LowerBound, m_UpperBound;
    float m_Speed;

public:
    VerticalMovingObstacle(b2World* world, b2Vec2 position, float lowerBound, float upperBound, float speed, const std::string& imagePath, float radius, float scale)
        : m_LowerBound(lowerBound / PTM), m_UpperBound(upperBound / PTM), m_Speed(speed) {
        
        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5);
        m_VisualComponent->m_Transform.scale = {scale, scale};
        m_VisualComponent->m_Transform.translation = {position.x * PTM, position.y * PTM};

        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody;
        bodyDef.position.Set(position.x, position.y);
        m_PhysicsBody = world->CreateBody(&bodyDef);

        b2CircleShape circleShape;
        circleShape.m_radius = radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 10.0f;
        m_PhysicsBody->CreateFixture(&fixtureDef);
        
        m_PhysicsBody->SetLinearVelocity(b2Vec2(0.0f, m_Speed)); // 初始速度
    }

    void Update() override {
        b2Vec2 pos = m_PhysicsBody->GetPosition();
        b2Vec2 vel = m_PhysicsBody->GetLinearVelocity();

        // 碰到邊界反向
        if (pos.y >= m_UpperBound && vel.y > 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(0.0f, -std::abs(m_Speed)));
        else if (pos.y <= m_LowerBound && vel.y < 0) m_PhysicsBody->SetLinearVelocity(b2Vec2(0.0f, std::abs(m_Speed)));

        m_VisualComponent->m_Transform.translation = {pos.x * PTM, pos.y * PTM};
    }
};

class CircularMovingObstacle : public Obstacle {
private:
    const float PTM = 30.0f;
    const float PI = 3.14159265f;
    b2Vec2 m_Center;
    float m_Radius;
    float m_AngularSpeed; // 弧度每秒
    float m_Angle;

public:
    CircularMovingObstacle(b2World* world, b2Vec2 center, float radius, float angularSpeed, const std::string& imagePath, float colRadius, float scale)
        : m_Center(center), m_Radius(radius / PTM), m_AngularSpeed(angularSpeed), m_Angle(0.0f) {
        
        float startX = m_Center.x + m_Radius;
        float startY = m_Center.y;

        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5);
        m_VisualComponent->m_Transform.scale = {scale, scale};
        m_VisualComponent->m_Transform.translation = {startX * PTM, startY * PTM};

        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody;
        bodyDef.position.Set(startX, startY);
        m_PhysicsBody = world->CreateBody(&bodyDef);

        b2CircleShape circleShape;
        circleShape.m_radius = colRadius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 10.0f;
        m_PhysicsBody->CreateFixture(&fixtureDef);
    }

    void Update() override {
        m_Angle += m_AngularSpeed * (1.0f / 60.0f);
        if (m_Angle > 2.0f * PI) m_Angle -= 2.0f * PI;
        else if (m_Angle < 0.0f) m_Angle += 2.0f * PI;

        float targetX = m_Center.x + m_Radius * std::cos(m_Angle);
        float targetY = m_Center.y + m_Radius * std::sin(m_Angle);

        b2Vec2 pos = m_PhysicsBody->GetPosition();
        b2Vec2 velocity(60.0f * (targetX - pos.x), 60.0f * (targetY - pos.y));
        m_PhysicsBody->SetLinearVelocity(velocity);

        m_VisualComponent->m_Transform.translation = {pos.x * PTM, pos.y * PTM};
    }
};

class Figure8MovingObstacle : public Obstacle {
private:
    const float PTM = 30.0f;
    const float PI = 3.14159265f;
    b2Vec2 m_Center;
    float m_AmpX; // X 振幅 (以像素為單位)
    float m_AmpY; // Y 振幅 (以像素為單位)
    float m_Speed; // 角速度 (弧度每秒)
    float m_Angle;

public:
    Figure8MovingObstacle(b2World* world, b2Vec2 center, float ampX, float ampY, float speed, const std::string& imagePath, float colRadius, float scale)
        : m_Center(center), m_AmpX(ampX / PTM), m_AmpY(ampY / PTM), m_Speed(speed), m_Angle(0.0f) {
        
        // 初始位置，假設 t=0
        float startX = m_Center.x;
        float startY = m_Center.y;

        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5);
        m_VisualComponent->m_Transform.scale = {scale, scale};
        m_VisualComponent->m_Transform.translation = {startX * PTM, startY * PTM};

        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody;
        bodyDef.position.Set(startX, startY);
        m_PhysicsBody = world->CreateBody(&bodyDef);

        b2CircleShape circleShape;
        circleShape.m_radius = colRadius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 10.0f;
        m_PhysicsBody->CreateFixture(&fixtureDef);
    }

    void Update() override {
        m_Angle += m_Speed * (1.0f / 60.0f);
        if (m_Angle > 2.0f * PI) m_Angle -= 2.0f * PI;
        else if (m_Angle < 0.0f) m_Angle += 2.0f * PI;

        // 直式八字形：x = A * sin(2t), y = B * sin(t)
        float targetX = m_Center.x + m_AmpX * std::sin(2.0f * m_Angle);
        float targetY = m_Center.y + m_AmpY * std::sin(m_Angle);

        b2Vec2 pos = m_PhysicsBody->GetPosition();
        b2Vec2 velocity(60.0f * (targetX - pos.x), 60.0f * (targetY - pos.y));
        m_PhysicsBody->SetLinearVelocity(velocity);

        m_VisualComponent->m_Transform.translation = {pos.x * PTM, pos.y * PTM};
    }
};

#endif