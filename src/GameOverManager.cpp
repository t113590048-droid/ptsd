#include "GameOverManager.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <string>
#include <cmath>

const float PTM = 30.0f;

GameOverManager::GameOverManager() {
    // Game Over 主標題 (稍微往上移一點)
    m_GameOverText = std::make_shared<Util::GameObject>();
    m_GameOverText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        100,
        "GAME OVER",
        Util::Color(255, 50, 50, 255)// 紅
    ));
    m_GameOverText->SetZIndex(100);
    m_GameOverText->m_Transform.translation = {0.0f, 80.0f};

    // 重新開始提示 (稍微往下移一點)
    m_RestartText = std::make_shared<Util::GameObject>();
    m_RestartText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        40,
        "Press R to Menu",
        Util::Color(255, 255, 255, 255) // 白
    ));
    m_RestartText->SetZIndex(100);
    m_RestartText->m_Transform.translation = {0.0f, -80.0f};
}
void GameOverManager::CheckGameOver(const std::vector<std::shared_ptr<Fruit>>& fruits, int currentScore) {
    if (m_IsGameOver) return;
    float highestTopY = -1000.0f;
    for (const auto& fruit : fruits) {
        b2Body* body = fruit->GetBody();
        if (!body) continue;
        float centerY = body->GetPosition().y * PTM;
        float radius = 0.0f;
        b2Fixture* fixture = body->GetFixtureList();
        if (fixture) {
            radius = fixture->GetShape()->m_radius * PTM;
        }
        float topY = centerY + radius;
        if (topY > highestTopY) {
            highestTopY = topY;
        }
    }
    if (highestTopY > m_DeathLineY) {
        m_FramesAboveLine++;
        if (m_FramesAboveLine >= 300) {
            m_IsGameOver = true; // 宣告死亡
            // 在死亡的瞬間，建立「最終分數」的文字物件
            m_FinalScoreText = std::make_shared<Util::GameObject>();
            m_FinalScoreText->SetDrawable(std::make_shared<Util::Text>(
                "Resources/material/font/Roboto-Regular.ttf",
                60,
                "Score: " + std::to_string(currentScore),
                Util::Color(255, 215, 0, 255) // 金黃色
            ));
            m_FinalScoreText->SetZIndex(100);
            m_FinalScoreText->m_Transform.translation = {0.0f, -10.0f};
        }
    } else {
        m_FramesAboveLine = 0;
    }
}
void GameOverManager::Draw() {
    if (m_IsGameOver) {
        if (m_GameOverText) m_GameOverText->Draw();
        if (m_FinalScoreText) m_FinalScoreText->Draw(); // 繪製分數
        if (m_RestartText) m_RestartText->Draw();
    }
}
bool GameOverManager::IsReadyToReturnMenu() const {
    return m_IsGameOver && Util::Input::IsKeyDown(Util::Keycode::R);
}