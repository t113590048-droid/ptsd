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
            radius = 18.0f;
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
            radius = 37.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Apple:
            imagePath += "apple.png";
            radius = 47.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Pear:
            imagePath += "pear.png";
            radius = 58.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Peach:
            imagePath += "peach.png";
            radius = 67.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Pineapple:
            imagePath += "pineapple.png";
            radius = 75.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Melon:
            imagePath += "melon.png";
            radius = 93.0f;
            scale = 0.6f;
            break;
        case FruitLevel::Watermelon:
            imagePath += "watermelon.png";
            radius = 108.0f;
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
FruitLevel FruitFactory::GetRandomFruit(LevelType levelType, int subLevel) {
    // ✨ 如果不是普通模式，直接執行原版邏輯 (返回所有水果)
    if (levelType != LevelType::NORMAL) {
        return static_cast<FruitLevel>(std::rand() % 5); // 回傳 0~4 級
    }

    // 以下是普通模式的難度邏輯
    std::vector<FruitLevel> pool;
    if (subLevel == 2) pool = {FruitLevel::Cherry, FruitLevel::Strawberry, FruitLevel::Grape, FruitLevel::tangerine};
    else if (subLevel == 3) pool = {FruitLevel::Cherry, FruitLevel::Grape, FruitLevel::Orange};
    else if (subLevel == 4) pool = {FruitLevel::Cherry, FruitLevel::Strawberry};
    else if (subLevel == 5) pool = {FruitLevel::Cherry};
    else pool = {FruitLevel::Cherry, FruitLevel::Strawberry, FruitLevel::Grape, FruitLevel::tangerine, FruitLevel::Orange};

    return pool[std::rand() % pool.size()];
}