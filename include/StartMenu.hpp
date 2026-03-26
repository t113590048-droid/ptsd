#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <memory>

class StartMenu {
public:
    // 定義三個按鈕的狀態
    enum class MenuOption {
        GAME_START,
        MY_SCORE,
        HOW_TO_PLAY
    };

    StartMenu();
    void Update();

    MenuOption GetCurrentOption() const { return m_CurrentOption; }

private:
    MenuOption m_CurrentOption = MenuOption::GAME_START;

    // 宣告 UI 遊戲物件
    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Logo;
    std::shared_ptr<Util::GameObject> m_BtnStart;
    std::shared_ptr<Util::GameObject> m_BtnScore;
    std::shared_ptr<Util::GameObject> m_BtnHowTo;

    // 更新按鈕圖片的方法
    void UpdateButtonVisuals();
};

#endif // STARTMENU_HPP