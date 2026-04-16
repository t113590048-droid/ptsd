#include "PauseManager.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

PauseManager::PauseManager() {
    // 1. PAUSE 主標題
    m_PauseTitle = std::make_shared<Util::GameObject>();
    m_PauseTitle->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        100,
        "PAUSE",
        Util::Color(255, 255, 255, 255)
    ));
    m_PauseTitle->SetZIndex(100);
    m_PauseTitle->m_Transform.translation = {0.0f, 80.0f};
    // 2. 繼續遊戲提示
    m_ResumeText = std::make_shared<Util::GameObject>();
    m_ResumeText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        40,
        "Press P to Resume",
        Util::Color(255, 255, 255, 255)
    ));
    m_ResumeText->SetZIndex(100);
    m_ResumeText->m_Transform.translation = {0.0f, -20.0f};
    // 3. 回主選單提示
    m_RestartText = std::make_shared<Util::GameObject>();
    m_RestartText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        40,
        "Press R to Menu",
        Util::Color(255, 255, 255, 255)
    ));
    m_RestartText->SetZIndex(100);
    m_RestartText->m_Transform.translation = {0.0f, -80.0f};
}
void PauseManager::Update() {
    // 偵測按下 P 鍵，切換暫停狀態
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        m_IsPaused = !m_IsPaused;
    }
}
void PauseManager::Draw() {
    if (m_IsPaused) {
        if (m_PauseTitle) m_PauseTitle->Draw();
        if (m_ResumeText) m_ResumeText->Draw();
        if (m_RestartText) m_RestartText->Draw();
    }
}
bool PauseManager::IsReadyToReturnMenu() const {
    // 只有在暫停狀態下，按下 R 才會回傳 true
    return m_IsPaused && Util::Input::IsKeyDown(Util::Keycode::R);
}