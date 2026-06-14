#include "Gameplay.hpp"
#include "ContactListener.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Util/Logger.hpp"

const float PTM = 30.0f;

Gameplay::Gameplay(LevelType level) : m_CurrentLevelType(level) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    m_CurrentFruitLevel = FruitFactory::GetRandomFruit();
    m_NextFruitLevel = FruitFactory::GetRandomFruit();

    m_DropSound = std::make_shared<Util::SFX>("Resources/material/music/drop.wav");
    m_MergeSound = std::make_shared<Util::SFX>("Resources/material/music/remove.wav");

    m_DropSound->SetVolume(30);
    m_MergeSound->SetVolume(50);

    m_UIManager = std::make_unique<UIManager>();
    m_UIManager->UpdateNextFruit(m_NextFruitLevel);

    m_CloudManager = std::make_unique<Cloud>();
    m_CloudManager->SetCurrentFruit(m_CurrentFruitLevel);

    b2Vec2 gravity(0.0f, -30.0f);
    m_World = std::make_unique<b2World>(gravity);
    m_ContactListener = std::make_unique<ContactListener>(this);
    m_World->SetContactListener(m_ContactListener.get());

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f / PTM, -328.0f / PTM);
    m_GroundBody = m_World->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(1000.0f / PTM, 10.0f / PTM);
    m_GroundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef leftWallDef;
    leftWallDef.position.Set(-232.0f / PTM, 0.0f / PTM);
    m_LeftWallBody = m_World->CreateBody(&leftWallDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f / PTM, 1000.0f / PTM);
    m_LeftWallBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallDef;
    rightWallDef.position.Set(237.0f / PTM, 0.0f / PTM);
    m_RightWallBody = m_World->CreateBody(&rightWallDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(10.0f / PTM, 1000.0f / PTM);
    m_RightWallBody->CreateFixture(&rightWallBox, 0.0f);

    if (level == LevelType::STATIC_PEGS) {
        m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(-100.0f / PTM, 0.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
        m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(100.0f / PTM, 50.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
    }
    else if (level == LevelType::MOVING_PLATFORMS) {
        m_Obstacles.push_back(std::make_shared<MovingObstacle>(
            m_World.get(), b2Vec2(0.0f / PTM, 30.0f / PTM), -150.0f, 150.0f, 3.0f, "Resources/material/platform.png", 20.0f / PTM, 0.15));
    }
    else if (level == LevelType::PORTAL) {
        m_Obstacles.push_back(std::make_shared<PortalObstacle>(b2Vec2(-150.0f / PTM, 100.0f / PTM), "Resources/material/portal_1.png", 0.13f));
        m_Obstacles.push_back(std::make_shared<PortalObstacle>(b2Vec2(150.0f / PTM, -100.0f / PTM), "Resources/material/portal_2.png", 0.13f));
    }

    m_PauseManager = std::make_unique<PauseManager>();
    m_GameOverManager = std::make_unique<GameOverManager>();
    m_CheatManager = std::make_unique<CheatManager>();

    // ✨✨✨ 初始化開場動畫 (布幕邏輯現在全都在這裡面了)
    m_OpeningAnimation = std::make_unique<OpeningAnimation>();
}

Gameplay::~Gameplay() = default;

void Gameplay::RegisterMerge(Fruit* a, Fruit* b) {
    for (const auto& pair : m_ToMerge) {
        if ((pair.a == a && pair.b == b) || (pair.a == b && pair.b == a)) return;
    }
    m_ToMerge.push_back({a, b});
}

void Gameplay::HandleMerges() {
    if (m_ToMerge.empty()) return;
    int scoreTable[] = {0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66};

    for (auto& pair : m_ToMerge) {
        auto itA = std::find_if(m_Fruits.begin(), m_Fruits.end(), [&](auto& f){ return f.get() == pair.a; });
        auto itB = std::find_if(m_Fruits.begin(), m_Fruits.end(), [&](auto& f){ return f.get() == pair.b; });

        if (itA != m_Fruits.end() && itB != m_Fruits.end()) {
            if (pair.a->GetLevel() == FruitLevel::Watermelon) continue;

            b2Vec2 posA = pair.a->GetBody()->GetPosition();
            b2Vec2 posB = pair.b->GetBody()->GetPosition();
            b2Vec2 spawnPos((posA.x + posB.x) / 2.0f * PTM, (posA.y + posB.y) / 2.0f * PTM);
            FruitLevel nextLevel = static_cast<FruitLevel>((int)pair.a->GetLevel() + 1);

            int levelIdx = static_cast<int>(nextLevel);
            if (levelIdx >= 1 && levelIdx <= 11) {
                m_Score += scoreTable[levelIdx];
                m_UIManager->UpdateScore(m_Score);
                if (m_MergeSound) m_MergeSound->Play();
            }
            b2Vec2 b2SpawnPos(spawnPos.x / PTM, spawnPos.y / PTM);

            m_World->DestroyBody(pair.a->GetBody());
            m_World->DestroyBody(pair.b->GetBody());
            m_Fruits.erase(std::remove(m_Fruits.begin(), m_Fruits.end(), *itA), m_Fruits.end());
            itB = std::find_if(m_Fruits.begin(), m_Fruits.end(), [&](auto& f){ return f.get() == pair.b; });
            m_Fruits.erase(std::remove(m_Fruits.begin(), m_Fruits.end(), *itB), m_Fruits.end());

            float blastRadius = (200.0f + (levelIdx * 15.0f)) / PTM;
            float blastPower = 8.0f + (levelIdx * 1.5f);
            ApplyExplosionForce(b2SpawnPos, blastRadius, blastPower);

            if (nextLevel <= FruitLevel::Watermelon) {
                m_Fruits.push_back(FruitFactory::CreateFruit(nextLevel, m_World.get(), spawnPos));
            }
        }
    }
    m_ToMerge.clear();
}

void Gameplay::Update() {
    if (m_GameOverManager->IsGameOver()) {
        if (m_CheatManager->IsOpen()) {
            m_CheatManager->Close();
        }

        m_GameOverManager->UpdateAnimation(m_Fruits);
        m_UIManager->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();
        m_GameOverManager->Draw();

        if (m_GameOverManager->IsReadyToReturnMenu()) m_WantsToReturnMenu = true;
        return;
    }

    m_CheatManager->Update();

    if (m_CheatManager->ConsumeScoreCheat()) {
        m_Score += 1000;
        m_UIManager->UpdateScore(m_Score);
        if (m_MergeSound) m_MergeSound->Play();
    }

    if (m_CheatManager->ConsumeGameoverCheat()) {
        m_GameOverManager->ForceGameOver(m_Fruits, m_Score);
        m_CheatManager->Close();
        return;
    }

    FruitLevel spawnLevel;
    float spawnX, spawnY;
    if (m_CheatManager->ConsumeFruitSpawn(spawnLevel, spawnX, spawnY)) {
        m_Fruits.push_back(FruitFactory::CreateFruit(spawnLevel, m_World.get(), b2Vec2(spawnX, spawnY)));
        if (m_DropSound) m_DropSound->Play();
        LOG_TRACE("Cheat Activated: Spawned Fruit {} at ({}, {})", static_cast<int>(spawnLevel), spawnX, spawnY);
    }

    if (m_CheatManager->IsOpen()) {
        m_UIManager->Draw();
        m_CloudManager->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();
        m_CheatManager->Draw();
        return;
    }

    m_PauseManager->Update();
    if (m_PauseManager->IsReadyToReturnMenu()) {
        m_WantsToReturnMenu = true;
        return;
    }

    if (m_PauseManager->IsPaused()) {
        m_UIManager->Draw();
        m_CloudManager->Draw();
        for (auto& obs : m_Obstacles) obs->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();
        m_PauseManager->Draw();
        return;
    }

    // ==========================================
    // ✨✨✨ 開場 Ready? Go!! 動畫與攔截器
    // ==========================================
    if (m_OpeningAnimation && !m_OpeningAnimation->IsFinished()) {
        m_OpeningAnimation->Update(1.0f / 60.0f);

        // ✨ 關鍵修改：改用 IsBlocking() 來判斷是否要鎖住玩家
        if (m_OpeningAnimation->IsBlocking()) {
            if (m_OpeningAnimation->IsCurtainClosed()) {
                m_UIManager->Draw();
                m_CloudManager->Draw();
                for (auto& obs : m_Obstacles) obs->Draw();
            }
            m_OpeningAnimation->Draw();
            return; // 👈 時間一到 4.0 秒就不 return 了，把操作權還給你！
        }
    }

    // ==========================================
    // 正常物理與遊戲邏輯
    // ==========================================
    m_World->Step(1.0f / 60.0f, 8, 3);
    HandleMerges();
    m_GameOverManager->CheckGameOver(m_Fruits, m_Score);
    m_CloudManager->Update();

    for (auto& obs : m_Obstacles) obs->Update();

    if (m_CurrentLevelType == LevelType::PORTAL && m_Obstacles.size() >= 2) {
        auto portalA = std::dynamic_pointer_cast<PortalObstacle>(m_Obstacles[0]);
        auto portalB = std::dynamic_pointer_cast<PortalObstacle>(m_Obstacles[1]);
        if (portalA && portalB) {
            for (auto& fruit : m_Fruits) {
                b2Vec2 pos = fruit->GetBody()->GetPosition();
                float dx = (pos.x * PTM) - portalA->GetPos().x;
                float dy = (pos.y * PTM) - portalA->GetPos().y;
                float distance = std::sqrt(dx * dx + dy * dy);
                if (distance < 40.0f) {
                    fruit->GetBody()->SetTransform(b2Vec2(portalB->GetPos().x / PTM, (portalB->GetPos().y - 50.0f) / PTM), fruit->GetBody()->GetAngle());
                }
            }
        }
    }

    if (m_CloudManager->IsDroppingFruit()) {
        if (m_DropSound) m_DropSound->Play();
        b2Vec2 spawnPos(m_CloudManager->GetDropX(), 280.0f);
        m_Fruits.push_back(FruitFactory::CreateFruit(m_CurrentFruitLevel, m_World.get(), spawnPos));
        m_CurrentFruitLevel = m_NextFruitLevel;
        m_NextFruitLevel = FruitFactory::GetRandomFruit();
        m_UIManager->UpdateNextFruit(m_NextFruitLevel);
        m_CloudManager->SetCurrentFruit(m_CurrentFruitLevel);
    }
    for (auto& fruit : m_Fruits) fruit->Update();

    // 正常遊戲繪製
    m_UIManager->Draw();
    m_CloudManager->Draw();
    for (auto& obs : m_Obstacles) obs->Draw();
    for (auto& fruit : m_Fruits) fruit->Draw();

    // ✨ 確保提早解除攔截後，剩下的 1 秒動畫(拉開布幕) 會疊在畫面上繼續播完！
    if (m_OpeningAnimation && !m_OpeningAnimation->IsFinished()) {
        m_OpeningAnimation->Draw();
    }
}

void Gameplay::ApplyExplosionForce(b2Vec2 explosionPos, float blastRadius, float blastPower) {
    for (auto& fruit : m_Fruits) {
        b2Body* body = fruit->GetBody();
        if (!body) continue;
        b2Vec2 fruitPos = body->GetPosition();
        b2Vec2 dir = fruitPos - explosionPos;
        float distance = dir.Length();
        if (distance < blastRadius && distance > 0.0f) {
            dir.Normalize();
            float forceFactor = 1.0f - (distance / blastRadius);
            float finalPower = blastPower * forceFactor;
            b2Vec2 impulse = finalPower * dir;
            body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        }
    }
}