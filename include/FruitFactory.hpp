// 這是概念藍圖
class FruitFactory {
public:
    // 只要告訴工廠：你要什麼等級、生在哪個世界、生在哪個座標
    static std::shared_ptr<Fruit> CreateFruit(FruitLevel level, b2World* world, b2Vec2 position);
};