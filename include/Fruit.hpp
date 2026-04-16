#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "Util/GameObject.hpp"
#include <box2d/box2d.h>
#include <memory>
#include <string>

// === 定義水果的等級 ===
enum class FruitLevel {
    Cherry = 0,
    Strawberry,
    Grape,
    tangerine,
    Orange,
    Apple,
    Pear,
    Peach,
    Pineapple,
    Melon,
    Watermelon
};

// === 水果類別 (肉體 + 靈魂) ===
class Fruit {
public:
    Fruit(b2World* world, b2Vec2 position, const std::string& imagePath, float radius, FruitLevel level, float scale);


    void Update();
    void Draw();
    b2Body* GetBody() const { return m_PhysicsBody; }
    FruitLevel GetLevel() const { return m_Level; } // 取得這顆水果的等級 (未來合成用)

private:
    std::shared_ptr<Util::GameObject> m_VisualComponent;
    b2Body* m_PhysicsBody;
    float m_Radius;
    FruitLevel m_Level; // 記住自己的等級
};

// === 水果工廠 (負責查字典生水果) ===
class FruitFactory {
public:
    static std::shared_ptr<Fruit> CreateFruit(FruitLevel level, b2World* world, b2Vec2 position);

    // 👇 新增這行：讓工廠來負責「骰」隨機水果
    static FruitLevel GetRandomFruit();
};

#endif // FRUIT_HPP