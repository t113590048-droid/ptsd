#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp"
#include "StaticObstacle.hpp"
#include "MovingObstacle.hpp"
#include "PortalObstacle.hpp"
#include "LevelType.hpp"
#include <memory>
#include <vector>
#include <box2d/box2d.h>
#include "GameOverManager.hpp"
#include "UIManager.hpp"
#include "Cloud.hpp"
#include "PauseManager.hpp"
#include "Util/SFX.hpp"
#include "CheatManager.hpp"
#include "OpeningAnimation.hpp"

class ContactListener;

class Gameplay {
public:
    Gameplay(LevelType level);
    ~Gameplay();
    void Update();
    void RegisterMerge(Fruit* a, Fruit* b);
    bool WantsToReturnMenu() const { return m_WantsToReturnMenu; }
    bool IsCurtainClosed() const { return m_OpeningAnimation && m_OpeningAnimation->IsCurtainClosed(); }

private:
    LevelType m_CurrentLevelType;
    std::unique_ptr<CheatManager> m_CheatManager;
    std::unique_ptr<UIManager> m_UIManager;
    std::unique_ptr<Cloud> m_CloudManager;
    FruitLevel m_CurrentFruitLevel;
    FruitLevel m_NextFruitLevel;

    std::vector<std::shared_ptr<Fruit>> m_Fruits;
    std::vector<std::shared_ptr<Obstacle>> m_Obstacles;

    std::unique_ptr<PauseManager> m_PauseManager;

    struct MergePair { Fruit* a; Fruit* b; };
    std::vector<MergePair> m_ToMerge;
    void HandleMerges();
    void ApplyExplosionForce(b2Vec2 explosionPos, float blastRadius, float blastPower);

    std::unique_ptr<b2World> m_World;
    std::unique_ptr<ContactListener> m_ContactListener;
    b2Body* m_GroundBody;
    b2Body* m_LeftWallBody;
    b2Body* m_RightWallBody;

    int m_Score = 0;

    std::unique_ptr<GameOverManager> m_GameOverManager;
    bool m_WantsToReturnMenu = false;

    std::shared_ptr<Util::SFX> m_DropSound;
    std::shared_ptr<Util::SFX> m_MergeSound;

    // 開場動畫
    std::unique_ptr<OpeningAnimation> m_OpeningAnimation;

};

#endif // GAMEPLAY_HPP