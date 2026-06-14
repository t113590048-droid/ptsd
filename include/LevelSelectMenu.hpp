#ifndef LEVELSELECTMENU_HPP
#define LEVELSELECTMENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "LevelType.hpp"
#include <vector>
#include <memory>

class LevelSelectMenu {
public:
    LevelSelectMenu();
    void Update();
    void Draw();

    bool IsLevelSelected() const { return m_IsSelected; }
    LevelType GetSelectedLevel() const { return m_SelectedLevel; }
    bool WantsToReturn() const { return m_WantsToReturn; }

private:
    void UpdateVisuals();

    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Title;

    std::vector<std::shared_ptr<Util::GameObject>> m_Options;
    // 直接記住 Text 元件，方便改顏色
    std::vector<std::shared_ptr<Util::Text>> m_OptionTexts;

    int m_CurrentIndex = 0;
    bool m_IsSelected = false;
    bool m_WantsToReturn = false;
    LevelType m_SelectedLevel = LevelType::NORMAL;
};

#endif