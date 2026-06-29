#include "LevelSelectMenu.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

LevelSelectMenu::LevelSelectMenu() {
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/gameplaybg.png"));
    m_Background->SetZIndex(0);

    m_Title = std::make_shared<Util::GameObject>();
    m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "SELECT LEVEL", Util::Color(255, 255, 255, 255)));
    m_Title->SetZIndex(1);
    m_Title->m_Transform.translation = {0.0f, 200.0f};

    // ==========================================
    // 1. 建立主選單選項
    // ==========================================
    m_OptionStrings = {
        "1. Normal Mode",
        "2. Static Pegs",
        "3. Moving Platforms",
        "4. Portal Mode",
        "Back to Menu"
    };

    float startY = 80.0f;
    for (int i = 0; i < 5; ++i) {
        auto textObj = std::make_shared<Util::GameObject>();
        auto textComp = std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 40, m_OptionStrings[i], Util::Color(200, 200, 200, 255));

        textObj->SetDrawable(textComp);
        textObj->SetZIndex(1);
        textObj->m_Transform.translation = {0.0f, startY - (i * 70.0f)};

        m_Options.push_back(textObj);
        m_OptionTexts.push_back(textComp);
    }

    // ==========================================
    // 2. 建立子選單選項 (5小關 + 返回)
    // ==========================================
    m_SubOptionStrings = { " ", " ", " ", " ", " ", " " };
    for (int i = 0; i < 6; ++i) {
        auto textObj = std::make_shared<Util::GameObject>();
        auto textComp = std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 40, m_SubOptionStrings[i], Util::Color(200, 200, 200, 255));

        textObj->SetDrawable(textComp);
        textObj->SetZIndex(1);
        textObj->m_Transform.translation = {0.0f, startY - (i * 70.0f)};

        m_SubOptions.push_back(textObj);
        m_SubOptionTexts.push_back(textComp);
    }

    UpdateVisuals();
}

void LevelSelectMenu::SetupSubMenuText() {
    std::string prefix;
    // 根據玩家選的模式，切換標題並設定關卡字首
    switch (m_CurrentIndex) {
        case 0: prefix = "1-"; m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "Normal Mode", Util::Color(255, 255, 255, 255))); break;
        case 1: prefix = "2-"; m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "Static Pegs", Util::Color(255, 255, 255, 255))); break;
        case 2: prefix = "3-"; m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "Moving Platforms", Util::Color(255, 255, 255, 255))); break;
        case 3: prefix = "4-"; m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "Portal Mode", Util::Color(255, 255, 255, 255))); break;
    }

    // 動態填寫 1~5 關的文字
    for (int i = 0; i < 5; ++i) {
        m_SubOptionStrings[i] = "Stage " + prefix + std::to_string(i + 1);
    }
    m_SubOptionStrings[5] = "Back to Modes";
}

void LevelSelectMenu::Update() {
    // ✨ 狀態一：正在選主模式
    if (m_State == MenuState::MAIN_MENU) {
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            m_CurrentIndex = (m_CurrentIndex > 0) ? m_CurrentIndex - 1 : 4;
            UpdateVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            m_CurrentIndex = (m_CurrentIndex < 4) ? m_CurrentIndex + 1 : 0;
            UpdateVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_CurrentIndex == 4) {
                m_WantsToReturn = true; // 退出到 StartMenu
            } else {
                m_State = MenuState::SUB_MENU; // ✨ 切換到子關卡狀態
                m_SubIndex = 0;
                SetupSubMenuText();
                UpdateVisuals();
            }
        }
    }
    // ✨ 狀態二：正在選 1~5 小關
    else if (m_State == MenuState::SUB_MENU) {
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            m_SubIndex = (m_SubIndex > 0) ? m_SubIndex - 1 : 5;
            UpdateVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            m_SubIndex = (m_SubIndex < 5) ? m_SubIndex + 1 : 0;
            UpdateVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_SubIndex == 5) {
                // 返回主模式選單
                m_State = MenuState::MAIN_MENU;
                m_Title->SetDrawable(std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 60, "SELECT LEVEL", Util::Color(255, 255, 255, 255)));
                UpdateVisuals();
            } else {
                // 正式開始遊戲！記錄大關與小關
                m_SelectedLevel = static_cast<LevelType>(m_CurrentIndex);
                m_SelectedSubLevel = m_SubIndex + 1; // 取得 1~5
                m_IsSelected = true;
            }
        }
    }
}

void LevelSelectMenu::Draw() {
    if (m_Background) m_Background->Draw();
    if (m_Title) m_Title->Draw();

    if (m_State == MenuState::MAIN_MENU) {
        for (auto& opt : m_Options) opt->Draw();
    } else if (m_State == MenuState::SUB_MENU) {
        for (auto& opt : m_SubOptions) opt->Draw();
    }
}

void LevelSelectMenu::UpdateVisuals() {
    if (m_State == MenuState::MAIN_MENU) {
        for (int i = 0; i < 5; ++i) {
            auto color = (i == m_CurrentIndex) ? Util::Color(255, 215, 0, 255) : Util::Color(150, 150, 150, 255);
            auto textComp = std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 40, m_OptionStrings[i], color);
            m_Options[i]->SetDrawable(textComp);
            m_OptionTexts[i] = textComp;
        }
    } else {
        for (int i = 0; i < 6; ++i) {
            auto color = (i == m_SubIndex) ? Util::Color(255, 215, 0, 255) : Util::Color(150, 150, 150, 255);
            auto textComp = std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 40, m_SubOptionStrings[i], color);
            m_SubOptions[i]->SetDrawable(textComp);
            m_SubOptionTexts[i] = textComp;
        }
    }
}