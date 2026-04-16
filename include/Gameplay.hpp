#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp"
#include <memory>
#include <vector>
#include <box2d/box2d.h>
#include "GameOverManager.hpp"
#include "UIManager.hpp"
#include "Cloud.hpp"
#include "PauseManager.hpp"

class ContactListener;
class Gameplay {
public:
    Gameplay();
    ~Gameplay();
    void Update();
    void RegisterMerge(Fruit* a, Fruit* b);
    bool WantsToReturnMenu() const { return m_WantsToReturnMenu; }
private:
    std::unique_ptr<UIManager> m_UIManager;
    std::unique_ptr<Cloud> m_CloudManager;
    FruitLevel m_CurrentFruitLevel;
    FruitLevel m_NextFruitLevel;

    std::vector<std::shared_ptr<Fruit>> m_Fruits;
    std::unique_ptr<PauseManager> m_PauseManager;

    struct MergePair { Fruit* a; Fruit* b; };
    std::vector<MergePair> m_ToMerge;
    void HandleMerges();

    std::unique_ptr<b2World> m_World;
    std::unique_ptr<ContactListener> m_ContactListener;
    b2Body* m_GroundBody;
    b2Body* m_LeftWallBody;
    b2Body* m_RightWallBody;

    int m_Score = 0;

    std::unique_ptr<GameOverManager> m_GameOverManager;
    bool m_WantsToReturnMenu = false;
};

#endif