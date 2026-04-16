#include "StartMenu.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

StartMenu::StartMenu() {
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/gameplaybg.png"));
    m_Background->SetZIndex(0);

    m_Logo = std::make_shared<Util::GameObject>();
    m_Logo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/logo.png"));
    m_Logo->SetZIndex(1);
    m_Logo->m_Transform.translation = {0.0f, 150.0f};

    m_BtnStart = std::make_shared<Util::GameObject>();
    m_BtnStart->SetZIndex(1);
    m_BtnStart->m_Transform.translation = {0.0f, 0.0f};

    m_BtnScore = std::make_shared<Util::GameObject>();
    m_BtnScore->SetZIndex(1);
    m_BtnScore->m_Transform.translation = {0.0f, -100.0f};

    m_BtnHowTo = std::make_shared<Util::GameObject>();
    m_BtnHowTo->SetZIndex(1);
    m_BtnHowTo->m_Transform.translation = {0.0f, -200.0f};

    // 初始化教學畫面物件 (ZIndex 設為 10，確保蓋在主選單上面)
    m_HowToPlayScreen = std::make_shared<Util::GameObject>();
    m_HowToPlayScreen->SetZIndex(10);
    m_HowToPlayScreen->m_Transform.translation = {0.0f, 0.0f};

    UpdateButtonVisuals();
}
void StartMenu::Update() {
    // ==========================================
    // 狀態 1：如果在「教學畫面」中
    // ==========================================
    if (m_IsShowingHowToPlay) {
        // 按 D 鍵：下一頁
        if (Util::Input::IsKeyUp(Util::Keycode::D)) {
            if (m_HowToPlayPage < 3) {
                m_HowToPlayPage++;
                UpdateHowToPlayVisuals();
            }
        }
        // 按 A 鍵：上一頁
        if (Util::Input::IsKeyUp(Util::Keycode::A)) {
            if (m_HowToPlayPage > 1) {
                m_HowToPlayPage--;
                UpdateHowToPlayVisuals();
            }
        }
        // 按下空白鍵或 ESC 鍵：關閉教學畫面
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_IsShowingHowToPlay = false;
        }
    }
    // ==========================================
    // 狀態 2：如果在「主選單」中
    // ==========================================
    else {
        // W 鍵：往上
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::GAME_START;
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
                m_CurrentOption = MenuOption::MY_SCORE;
            }
            UpdateButtonVisuals();
        }
        // S 鍵：往下
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                m_CurrentOption = MenuOption::MY_SCORE;
            } else if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::HOW_TO_PLAY;
            }
            UpdateButtonVisuals();
        }
        // 空白鍵：選擇
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                LOG_TRACE("Game Started!");
                m_IsGameStarted = true;
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
                // 開啟教學畫面，並將頁數初始化為第 1 頁
                m_IsShowingHowToPlay = true;
                m_HowToPlayPage = 1;
                UpdateHowToPlayVisuals();
            }
        }
    }
    // --- 繪製畫面 ---
    if (m_Background) m_Background->Draw();
    if (m_Logo) m_Logo->Draw();
    if (m_BtnStart) m_BtnStart->Draw();
    if (m_BtnScore) m_BtnScore->Draw();
    if (m_BtnHowTo) m_BtnHowTo->Draw();
    // 如果處於教學模式，就把教學圖片畫在最上層
    if (m_IsShowingHowToPlay && m_HowToPlayScreen) {
        m_HowToPlayScreen->Draw();
    }
}
// 原本的按鈕圖片更新保持不變
void StartMenu::UpdateButtonVisuals() {
    if (m_CurrentOption == MenuOption::GAME_START) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton2.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton1.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton1.png"));
    }
    else if (m_CurrentOption == MenuOption::MY_SCORE) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton1.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton2.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton1.png"));
    }
    else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton1.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton1.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton2.png"));
    }
}
// 根據 m_HowToPlayPage 更換教學圖片
void StartMenu::UpdateHowToPlayVisuals() {
    if (m_HowToPlayPage == 1) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay1.png"));
    } else if (m_HowToPlayPage == 2) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay2.png"));
    } else if (m_HowToPlayPage == 3) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay3.png"));
    }
}