#include "Gameplay.hpp"
#include "ContactListener.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Util/Logger.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

const float PTM = 30.0f;

// ✅ 順序對調過，且加入了 subLevel 參數的傳遞
Gameplay::Gameplay(LevelType level, int subLevel) : m_CurrentSubLevel(subLevel), m_CurrentLevelType(level) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ✨✨✨ 關鍵修改 1 & 2：告訴工廠現在是第幾小關，決定開局的兩顆水果
    m_CurrentFruitLevel = FruitFactory::GetRandomFruit(m_CurrentLevelType, m_CurrentSubLevel);
    m_NextFruitLevel = FruitFactory::GetRandomFruit(m_CurrentLevelType, m_CurrentSubLevel);

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
        // ✨ 根據 subLevel 決定釘子擺放
        switch (m_CurrentSubLevel) {
            case 1: // 2-1：一根在正中央
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(0.0f / PTM, 0.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                break;

            case 2: // 2-2：兩根上下，中間留空間給西瓜 (假設西瓜直徑約 100)
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(0.0f / PTM, 120.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(0.0f / PTM, -120.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                break;

            case 3: // 2-3：兩根橫放劃分三等份 (釘子左右各一)
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(-100.0f / PTM, 0.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(100.0f / PTM, 0.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                break;

            case 4: // 2-4：最上面三根擋住入口 (假設 Y=200 是接近上方)
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(-120.0f / PTM, 200.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(0.0f / PTM, 200.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(120.0f / PTM, 200.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                break;

            case 5: // 2-5：五根均勻在底下
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(-120.0f / PTM, -140.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(120.0f / PTM, -140.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(0.0f / PTM, -20.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(-120.0f / PTM, 100.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                m_Obstacles.push_back(std::make_shared<StaticObstacle>(m_World.get(), b2Vec2(120.0f / PTM, 100.0f / PTM), "Resources/material/peg.png", 16.0f / PTM, 0.12f));
                break;
        }
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

    // ✨✨✨ 初始化過關介面 UI
    m_FirstWatermelonMerged = false;
    m_IsWinMenuOpen = false;

    // 建立過關文字提示 (使用你現有的字體路徑)
    m_WinUIText = std::make_shared<Util::GameObject>();
    m_WinUIText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/FOT-OEDKTRSTD-E.otf", 45,
        "Success!\n\n[ SPACE ] continue  /  [ R ] finish",
        Util::Color(255, 215, 0, 255) // 金黃色字體
    ));
    m_WinUIText->SetZIndex(100); // 確保圖層在最前面，不會被水果遮住
    m_WinUIText->m_Transform.translation = {0.0f, 0.0f}; // 居中顯示
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
                if (nextLevel == FruitLevel::Watermelon && !m_FirstWatermelonMerged) {
                    m_FirstWatermelonMerged = true; // 標記已經合過了，下次再合出來就不會再跳視窗
                    m_IsWinMenuOpen = true;         // 打開過關介面
                }
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
    // ✨✨✨ 全新新增：大西瓜過關介面攔截器
    // ==========================================
    if (m_IsWinMenuOpen) {
        // 1. 雖然遊戲暫停，但背景的 UI、雲朵和水果還是要持續畫出來
        m_UIManager->Draw();
        m_CloudManager->Draw();
        for (auto& obs : m_Obstacles) obs->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();

        // 2. 畫出過關提示文字
        if (m_WinUIText) m_WinUIText->Draw();

        // 3. 偵測玩家輸入
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_IsWinMenuOpen = false; // 關閉介面，繼續遊玩！
        }
        if (Util::Input::IsKeyUp(Util::Keycode::R)) {
            m_WantsToReturnMenu = true; // 觸發返回主頁，App.cpp 會自動幫你換回 StartMenu
            m_IsWinMenuOpen = false;
        }

        return; // 👈 關鍵！攔截底下的物理運算與水果丟下邏輯，讓遊戲世界完全靜止
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
        // ✨✨✨ 關鍵修改 3：丟出水果後，告訴工廠根據第幾小關來產生下一顆
        m_NextFruitLevel = FruitFactory::GetRandomFruit(m_CurrentLevelType, m_CurrentSubLevel);

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