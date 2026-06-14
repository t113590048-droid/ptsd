#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Util/GameObject.hpp"
#include <memory>
#include <box2d/box2d.h>

class Obstacle {
protected:
    std::shared_ptr<Util::GameObject> m_VisualComponent;
    b2Body* m_PhysicsBody = nullptr;

public:
    virtual ~Obstacle() = default;
    
    // 虛擬函式：讓未來的移動障礙物可以覆寫
    virtual void Update() = 0; 
    
    // 共用的繪圖函式
    virtual void Draw() {
        if (m_VisualComponent) m_VisualComponent->Draw();
    }
};

#endif // OBSTACLE_HPP