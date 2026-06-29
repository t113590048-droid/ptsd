#ifndef FRUITFACTORY_HPP
#define FRUITFACTORY_HPP

#include "Fruit.hpp"
#include <memory>
#include <box2d/box2d.h>

class FruitFactory {
public:
    static std::shared_ptr<Fruit> CreateFruit(FruitLevel level, b2World* world, b2Vec2 pos);
    
    // ✨ 新增 subLevel 參數，預設為 1
    static FruitLevel GetRandomFruit(int subLevel = 1); 
};

#endif