#include "Gameplay.hpp"
#include "ContactListener.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

const float PTM = 30.0f;

Gameplay::Gameplay() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    m_CurrentFruitLevel = FruitFactory::GetRandomFruit();
    m_NextFruitLevel = FruitFactory::GetRandomFruit();
    // 預先載入音效檔案
    m_DropSound = std::make_shared<Util::SFX>("Resources/material/music/drop.wav");
    m_MergeSound = std::make_shared<Util::SFX>("Resources/material/music/remove.wav");
    m_DropSound->SetVolume(30);
    m_MergeSound->SetVolume(60);
    // 1. 初始化 UIManager
    m_UIManager = std::make_unique<UIManager>();
    m_UIManager->UpdateNextFruit(m_NextFruitLevel);
    // 2. 初始化雲朵、瞄準線與目前拿在手上的水果
    m_CloudManager = std::make_unique<Cloud>();
    m_CloudManager->SetCurrentFruit(m_CurrentFruitLevel);
    // 3. 物理世界與邊界設定
    b2Vec2 gravity(0.0f, -30.0f);
    m_World = std::make_unique<b2World>(gravity);
    m_ContactListener = std::make_unique<ContactListener>(this);
    m_World->SetContactListener(m_ContactListener.get());

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f / PTM, -310.0f / PTM);
    m_GroundBody = m_World->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(1000.0f / PTM, 10.0f / PTM);
    m_GroundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef leftWallDef;
    leftWallDef.position.Set(-238.0f / PTM, 0.0f / PTM);
    m_LeftWallBody = m_World->CreateBody(&leftWallDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f / PTM, 1000.0f / PTM);
    m_LeftWallBody->CreateFixture(&leftWallBox, 0.0f);

    b2BodyDef rightWallDef;
    rightWallDef.position.Set(225.0f / PTM, 0.0f / PTM);
    m_RightWallBody = m_World->CreateBody(&rightWallDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(10.0f / PTM, 1000.0f / PTM);
    m_RightWallBody->CreateFixture(&rightWallBox, 0.0f);

    m_PauseManager = std::make_unique<PauseManager>();
    m_GameOverManager = std::make_unique<GameOverManager>();
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

            m_World->DestroyBody(pair.a->GetBody());
            m_World->DestroyBody(pair.b->GetBody());
            m_Fruits.erase(std::remove(m_Fruits.begin(), m_Fruits.end(), *itA), m_Fruits.end());
            itB = std::find_if(m_Fruits.begin(), m_Fruits.end(), [&](auto& f){ return f.get() == pair.b; });
            m_Fruits.erase(std::remove(m_Fruits.begin(), m_Fruits.end(), *itB), m_Fruits.end());

            if (nextLevel <= FruitLevel::Watermelon) {
                m_Fruits.push_back(FruitFactory::CreateFruit(nextLevel, m_World.get(), spawnPos));
            }
        }
    }
    m_ToMerge.clear();
}
void Gameplay::Update() {
    // 暫停邏輯 (偵測 P 鍵)
    m_PauseManager->Update();
    // 在暫停時按了 R，直接回首頁
    if (m_PauseManager->IsReadyToReturnMenu()) {
        m_WantsToReturnMenu = true;
        return;
    }
    // 遊戲結束攔截器
    if (m_GameOverManager->IsGameOver()) {
        m_UIManager->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();
        m_GameOverManager->Draw();
        if (m_GameOverManager->IsReadyToReturnMenu()) m_WantsToReturnMenu = true;
        return;
    }
    // 暫停攔截器
    if (m_PauseManager->IsPaused()) {
        // 暫停時：只畫出所有東西，但不執行物理跟雲朵
        m_UIManager->Draw();
        m_CloudManager->Draw();
        for (auto& fruit : m_Fruits) fruit->Draw();
        m_PauseManager->Draw(); // 畫出 PAUSE 等文字
        return; // 提早 return，中斷下面的遊戲邏輯
    }
    m_World->Step(1.0f / 60.0f, 8, 3);
    HandleMerges();
    m_GameOverManager->CheckGameOver(m_Fruits, m_Score);
    m_CloudManager->Update();
    if (m_CloudManager->IsDroppingFruit()) {
        // 播放掉落音效
        if (m_DropSound) m_DropSound->Play();
        b2Vec2 spawnPos(m_CloudManager->GetDropX(), 280.0f);
        m_Fruits.push_back(FruitFactory::CreateFruit(m_CurrentFruitLevel, m_World.get(), spawnPos));
        m_CurrentFruitLevel = m_NextFruitLevel;
        m_NextFruitLevel = FruitFactory::GetRandomFruit();
        m_UIManager->UpdateNextFruit(m_NextFruitLevel);
        m_CloudManager->SetCurrentFruit(m_CurrentFruitLevel);
    }
    for (auto& fruit : m_Fruits) fruit->Update();
    // 繪製正常畫面
    m_UIManager->Draw();
    m_CloudManager->Draw();
    for (auto& fruit : m_Fruits) fruit->Draw();
}