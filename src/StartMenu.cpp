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
    m_Logo->m_Transform.translation = {0.0f, 180.0f}; // 稍微往上提
    m_Logo->m_Transform.scale = {0.8f, 0.8f};         // 加上縮放比例避免太大

    // --- 遊戲開始按鈕 ---
    m_BtnStart = std::make_shared<Util::GameObject>();
    m_BtnStart->SetZIndex(1);
    m_BtnStart->m_Transform.translation = {0.0f, 20.0f};
    m_BtnStart->m_Transform.scale = {0.5f, 0.5f};     // 寬高縮小一半

    // --- 我的分數按鈕 ---
    m_BtnScore = std::make_shared<Util::GameObject>();
    m_BtnScore->SetZIndex(1);
    m_BtnScore->m_Transform.translation = {0.0f, -100.0f}; // 拉開間距
    m_BtnScore->m_Transform.scale = {0.5f, 0.5f};          // 寬高縮小一半

    // --- 說明按鈕 ---
    m_BtnHowTo = std::make_shared<Util::GameObject>();
    m_BtnHowTo->SetZIndex(1);
    m_BtnHowTo->m_Transform.translation = {0.0f, -220.0f}; // 拉開間距
    m_BtnHowTo->m_Transform.scale = {0.5f, 0.5f};          // 寬高縮小一半

    // --- 初始化教學畫面物件 ---
    m_HowToPlayScreen = std::make_shared<Util::GameObject>();
    m_HowToPlayScreen->SetZIndex(10);
    m_HowToPlayScreen->m_Transform.translation = {0.0f, 0.0f};
    // 如果圖檔太大修改倍率：
    m_HowToPlayScreen->m_Transform.scale = {0.666f, 0.666f};

    UpdateButtonVisuals();
}

void StartMenu::Update() {
    // ==========================================
    // 狀態 1：如果在「教學畫面」中
    // ==========================================
    if (m_IsShowingHowToPlay) {
        if (Util::Input::IsKeyUp(Util::Keycode::D)) {
            if (m_HowToPlayPage < 3) {
                m_HowToPlayPage++;
                UpdateHowToPlayVisuals();
            }
        }
        if (Util::Input::IsKeyUp(Util::Keycode::A)) {
            if (m_HowToPlayPage > 1) {
                m_HowToPlayPage--;
                UpdateHowToPlayVisuals();
            }
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_IsShowingHowToPlay = false;
        }
    }
    // ==========================================
    // 狀態 2：如果在「主選單」中
    // ==========================================
    else {
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::GAME_START;
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
                m_CurrentOption = MenuOption::MY_SCORE;
            }
            UpdateButtonVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                m_CurrentOption = MenuOption::MY_SCORE;
            } else if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::HOW_TO_PLAY;
            }
            UpdateButtonVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                LOG_TRACE("Game Started!");
                m_IsGameStarted = true;
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
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

    if (m_IsShowingHowToPlay && m_HowToPlayScreen) {
        m_HowToPlayScreen->Draw();
    }
}

// ==========================================
// ★ 注意：如果你已經把圖片替換成了官方版
// 記得將這邊的檔名改成新的檔名！
// 例如將 howtoplaybutton1.png 改成 btn_htp.png
// ==========================================
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

void StartMenu::UpdateHowToPlayVisuals() {
    if (m_HowToPlayPage == 1) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay1.png"));
    } else if (m_HowToPlayPage == 2) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay2.png"));
    } else if (m_HowToPlayPage == 3) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay3.png"));
    }
}