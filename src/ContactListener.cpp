#include "ContactListener.hpp"
#include "Gameplay.hpp"
#include "Fruit.hpp"

ContactListener::ContactListener(Gameplay* gameplay) : m_Gameplay(gameplay) {}
void ContactListener::BeginContact(b2Contact* contact) {
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
    // 安全檢查：確保兩個剛體都有掛載 UserData (也就是 Fruit 的指標)
    if (bodyA->GetUserData().pointer == 0 || bodyB->GetUserData().pointer == 0) return;

    Fruit* fruitA = reinterpret_cast<Fruit*>(bodyA->GetUserData().pointer);
    Fruit* fruitB = reinterpret_cast<Fruit*>(bodyB->GetUserData().pointer);

    // 邏輯判定：如果是兩顆不同的水果，且等級相同
    if (fruitA && fruitB && fruitA != fruitB) {
        if (fruitA->GetLevel() == fruitB->GetLevel()) {
            m_Gameplay->RegisterMerge(fruitA, fruitB);
        }
    }
}