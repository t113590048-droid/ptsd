#ifndef LEVELSELECTMENU_HPP
#define LEVELSELECTMENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "LevelType.hpp"
#include <vector>
#include <memory>
#include <string>

class LevelSelectMenu {
public:
    // ✨ 定義選單目前的狀態
    enum class MenuState {
        MAIN_MENU, // 選擇四大模式
        SUB_MENU   // 選擇 1~5 小關
    };

    LevelSelectMenu();
    void Update();
    void Draw();

    bool IsLevelSelected() const { return m_IsSelected; }
    LevelType GetSelectedLevel() const { return m_SelectedLevel; }

    // ✨ 新增：讓外部可以取得玩家選了第幾小關 (1~5)
    int GetSelectedSubLevel() const { return m_SelectedSubLevel; }

    bool WantsToReturn() const { return m_WantsToReturn; }

private:
    void UpdateVisuals();
    void SetupSubMenuText(); // ✨ 新增：動態設定子關卡的文字

    MenuState m_State = MenuState::MAIN_MENU;

    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Title;

    // --- 主選單 (模式選擇) ---
    std::vector<std::shared_ptr<Util::GameObject>> m_Options;
    std::vector<std::shared_ptr<Util::Text>> m_OptionTexts;
    std::vector<std::string> m_OptionStrings;
    int m_CurrentIndex = 0; // 主選單的光標位置

    // --- 子選單 (小關選擇) ---
    std::vector<std::shared_ptr<Util::GameObject>> m_SubOptions;
    std::vector<std::shared_ptr<Util::Text>> m_SubOptionTexts;
    std::vector<std::string> m_SubOptionStrings;
    int m_SubIndex = 0;     // 子選單的光標位置

    bool m_IsSelected = false;
    bool m_WantsToReturn = false;

    LevelType m_SelectedLevel = LevelType::NORMAL;
    int m_SelectedSubLevel = 1; // 預設第 1 小關
};

#endif