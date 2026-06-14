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

    std::vector<std::string> optionTexts = {
        "1. Normal Mode",
        "2. Static Pegs",
        "3. Moving Platforms",
        "4. Portal Mode",
        "Back to Menu"
    };

    float startY = 80.0f;
    for (int i = 0; i < 5; ++i) {
        auto textObj = std::make_shared<Util::GameObject>();
        auto textComp = std::make_shared<Util::Text>("Resources/material/font/Roboto-Regular.ttf", 40, optionTexts[i], Util::Color(200, 200, 200, 255));

        textObj->SetDrawable(textComp);
        textObj->SetZIndex(1);
        textObj->m_Transform.translation = {0.0f, startY - (i * 70.0f)};

        m_Options.push_back(textObj);
        m_OptionTexts.push_back(textComp); // 存入指標
    }
    UpdateVisuals();
}

void LevelSelectMenu::Update() {
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
            m_WantsToReturn = true;
        } else {
            m_SelectedLevel = static_cast<LevelType>(m_CurrentIndex);
            m_IsSelected = true;
        }
    }
}

void LevelSelectMenu::Draw() {
    if (m_Background) m_Background->Draw();
    if (m_Title) m_Title->Draw();
    for (auto& opt : m_Options) opt->Draw();
}

void LevelSelectMenu::UpdateVisuals() {
    for (int i = 0; i < 5; ++i) {
        auto color = (i == m_CurrentIndex) ? Util::Color(255, 215, 0, 255) : Util::Color(150, 150, 150, 255);
        m_OptionTexts[i]->SetColor(color); // 直接設定顏色，不再呼叫 GetDrawable
    }
}