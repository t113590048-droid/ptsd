#ifndef PORTALOBSTACLE_HPP
#define PORTALOBSTACLE_HPP

#include "Obstacle.hpp"
#include "Util/Image.hpp"

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
#endif