#ifndef APP_HPP
#define APP_HPP

#include "StartMenu.hpp"
#include "LevelSelectMenu.hpp"
#include "Gameplay.hpp"
#include <memory>
#include "Util/BGM.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();
    void Update();
    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    State m_CurrentState = State::START;

    // ★ 變數必須放在 class 的 private 區塊裡面，不能放在外面！
    std::shared_ptr<StartMenu> m_StartMenu;
    std::shared_ptr<LevelSelectMenu> m_LevelSelectMenu;
    std::shared_ptr<Gameplay> m_Gameplay;
    std::shared_ptr<Util::BGM> m_BGM;
    int m_Volume = 64;
};

#endif // APP_HPP