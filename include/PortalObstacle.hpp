#ifndef PORTALOBSTACLE_HPP
#define PORTALOBSTACLE_HPP

#include "Obstacle.hpp"
#include "Util/Image.hpp"
#include <string>

class PortalObstacle : public Obstacle {
public:
    PortalObstacle(b2Vec2 position, const std::string& imagePath, float scale) {
        m_VisualComponent = std::make_shared<Util::GameObject>();
        m_VisualComponent->SetDrawable(std::make_shared<Util::Image>(imagePath));
        m_VisualComponent->SetZIndex(5);
        m_VisualComponent->m_Transform.scale = {scale, scale};
        m_VisualComponent->m_Transform.translation = {position.x * 30.0f, position.y * 30.0f};
    }
    void Update() override {
        if (m_VisualComponent) {
            // 每次更新增加一點旋轉角度 (數字越大轉越快)
            m_VisualComponent->m_Transform.rotation += 0.02f;

            // 防止數值無限變大的保護機制 (超過一圈就歸零)
            if (m_VisualComponent->m_Transform.rotation > 6.28318f) {
                m_VisualComponent->m_Transform.rotation -= 6.28318f;
            }
        }
    } // 無物理剛體，純視覺
    glm::vec2 GetPos() const { return m_VisualComponent->m_Transform.translation; }
};

class MovingPortalObstacle : public PortalObstacle {
private:
    float m_LeftBound, m_RightBound;
    float m_Speed;
    int m_Direction; // 1: 右, -1: 左

public:
    MovingPortalObstacle(b2Vec2 position, float leftBound, float rightBound, float speed, const std::string& imagePath, float scale, int initialDirection = 1)
        : PortalObstacle(position, imagePath, scale), m_LeftBound(leftBound), m_RightBound(rightBound), m_Speed(speed), m_Direction(initialDirection) {}

    void Update() override {
        // 旋轉傳送門
        PortalObstacle::Update();

        // 水平移動
        glm::vec2 pos = m_VisualComponent->m_Transform.translation;
        pos.x += m_Direction * m_Speed;
        if (pos.x >= m_RightBound) {
            pos.x = m_RightBound;
            m_Direction = -1;
        } else if (pos.x <= m_LeftBound) {
            pos.x = m_LeftBound;
            m_Direction = 1;
        }
        m_VisualComponent->m_Transform.translation = pos;
    }
};

#endif