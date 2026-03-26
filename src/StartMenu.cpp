#include "StartMenu.hpp"
#include "Util/Image.hpp" // 解決 'Image' is not a member of 'Util' 的問題
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

    UpdateButtonVisuals();
}

void StartMenu::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::UP)) {
        if (m_CurrentOption == MenuOption::MY_SCORE) {
            m_CurrentOption = MenuOption::GAME_START;
        } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
            m_CurrentOption = MenuOption::MY_SCORE;
        }
        UpdateButtonVisuals();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::DOWN)) {
        if (m_CurrentOption == MenuOption::GAME_START) {
            m_CurrentOption = MenuOption::MY_SCORE;
        } else if (m_CurrentOption == MenuOption::MY_SCORE) {
            m_CurrentOption = MenuOption::HOW_TO_PLAY;
        }
        UpdateButtonVisuals();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
        if (m_CurrentOption == MenuOption::GAME_START) {
            LOG_TRACE("Game Started!");
        }
    }

    if (m_Background) m_Background->Draw();
    if (m_Logo) m_Logo->Draw();
    if (m_BtnStart) m_BtnStart->Draw();
    if (m_BtnScore) m_BtnScore->Draw();
    if (m_BtnHowTo) m_BtnHowTo->Draw();
}

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