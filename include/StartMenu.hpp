#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include "Util/GameObject.hpp"
#include <memory>

class StartMenu {
public:
    enum class MenuOption {
        GAME_START,
        MY_SCORE,
        HOW_TO_PLAY
    };
    StartMenu();
    void Update();
    MenuOption GetCurrentOption() const { return m_CurrentOption; }
    bool IsGameStarted() const { return m_IsGameStarted; }
private:
    MenuOption m_CurrentOption = MenuOption::GAME_START;
    bool m_IsGameStarted = false;
    // 用來記錄是否正在顯示教學畫面，以及目前在第幾頁
    bool m_IsShowingHowToPlay = false;
    int m_HowToPlayPage = 1;
    // 宣告 UI 遊戲物件
    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Logo;
    std::shared_ptr<Util::GameObject> m_BtnStart;
    std::shared_ptr<Util::GameObject> m_BtnScore;
    std::shared_ptr<Util::GameObject> m_BtnHowTo;
    std::shared_ptr<Util::GameObject> m_HowToPlayScreen;
    void UpdateButtonVisuals();
    // 用來更新教學圖片的方法
    void UpdateHowToPlayVisuals();
};
#endif