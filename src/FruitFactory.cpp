#include "Fruit.hpp"
#include <memory>
#include <iostream>
#include <cstdlib>

// ==========================================
// 水果工廠的實作 (圖鑑字典)
// ==========================================
std::shared_ptr<Fruit> FruitFactory::CreateFruit(FruitLevel level, b2World* world, b2Vec2 position) {
    std::string imagePath = "Resources/material/fruit/";
    float radius = 0.0f;
    float scale = 1.0f;

    switch (level) {
        case FruitLevel::Cherry:
            imagePath += "cherry.png";
            radius = 16.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Strawberry:
            imagePath += "strawberry.png";
            radius = 20.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Grape:
            imagePath += "grape.png";
            radius = 27.0f;
            scale = 0.6f;
            break;
        case FruitLevel::tangerine:
            imagePath += "tangerine.png";
            radius = 30.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Orange:
            imagePath += "orange.png";
            radius = 40.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Apple:
            imagePath += "apple.png";
            radius = 50.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Pear:
            imagePath += "pear.png";
            radius = 58.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Peach:
            imagePath += "peach.png";
            radius = 70.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Pineapple:
            imagePath += "pineapple.png";
            radius = 79.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Melon:
            imagePath += "melon.png";
            radius = 100.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Watermelon:
            imagePath += "watermelon.png";
            radius = 116.0f;
            scale = 0.6f;
            break;
        default:
            std::cerr << "Error: Unknown FruitLevel!" << std::endl;
            imagePath += "cherry.png"; // 預防錯誤，預設給櫻桃
            radius = 20.0f;
            scale = 0.6f;
            break;
    }
    return std::make_shared<Fruit>(world, position, imagePath, radius, level, scale);
}
// ==========================================
// 隨機抽取要掉落的水果 (限定前 5 級)
// ==========================================
FruitLevel FruitFactory::GetRandomFruit() {
    // 產生 0 ~ 4 的隨機數 (Cherry ~ Orange)
    int randomVal = std::rand() % 5;
    //int randomVal = 10;
    return static_cast<FruitLevel>(randomVal);
}