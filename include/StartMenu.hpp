#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include "Util/GameObject.hpp"
#include <memory>
#include <vector>
#include <string>

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
    // 用來記錄是否正在顯示個人排行榜/計分表
    bool m_IsShowingScoreboard = false;

    // 宣告 UI 遊戲物件
    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Logo;
    std::shared_ptr<Util::GameObject> m_BtnStart;
    std::shared_ptr<Util::GameObject> m_BtnScore;
    std::shared_ptr<Util::GameObject> m_BtnHowTo;
    std::shared_ptr<Util::GameObject> m_HowToPlayScreen;

    // 計分表 UI 遊戲物件
    std::shared_ptr<Util::GameObject> m_ScoreboardTitle;
    std::vector<std::shared_ptr<Util::GameObject>> m_ScoreboardLines;
    std::shared_ptr<Util::GameObject> m_ScoreboardReturnPrompt;

    void UpdateButtonVisuals();
    // 用來更新教學圖片的方法
    void UpdateHowToPlayVisuals();
    // 用來更新並載入計分表數據的方法
    void UpdateScoreboardVisuals();
};
#endif // STARTMENU_HPP