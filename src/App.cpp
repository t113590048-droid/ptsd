#include "App.hpp"
#include "StartMenu.hpp"
#include "Gameplay.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_StartMenu = std::make_shared<StartMenu>();// 實例化並載入選單
    m_Gameplay = nullptr;// 遊戲場景一開始先不載入 (設為空指標)
    m_CurrentState = State::UPDATE;
}
void App::Update() {
    // ==========================================
    // 場景切換狀態機
    // ==========================================
    // 狀態 A：如果選單還在，就更新選單
    if (m_StartMenu) {
        m_StartMenu->Update();
        // 檢查選單是否發出了「遊戲開始」的訊號
        if (m_StartMenu->IsGameStarted()) {
            LOG_TRACE("Switching to Gameplay Scene!");
            // 1. 載入並初始化 Gameplay 場景
            m_Gameplay = std::make_shared<Gameplay>();
            // 2. 把選單指標清空 (銷毀 StartMenu，釋放記憶體)
            m_StartMenu = nullptr;
        }
    }
    // 狀態 B：如果選單已經被銷毀，且遊戲場景存在，就更新遊戲場景
    else if (m_Gameplay) {
        m_Gameplay->Update();
        // 檢查遊戲是否發出「回主選單」的訊號 (死亡後按下R鍵)
        if (m_Gameplay->WantsToReturnMenu()) {
            LOG_TRACE("Game Over! Switching back to Start Menu!");
            // 1. 重新實例化主選單 (回到初始畫面)
            m_StartMenu = std::make_shared<StartMenu>();
            // 2. 銷毀遊戲場景 (觸發 ~Gameplay()，清空物理世界與水果)
            m_Gameplay = nullptr;
        }
    }
    // --------------------------------------------------------
    // Do not touch the code below as they serve the purpose for
    // closing the window.
    // --------------------------------------------------------
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
        }
}
void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}