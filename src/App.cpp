#include "App.hpp"
#include "StartMenu.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    // 1. 實例化並載入選單
    m_StartMenu = std::make_shared<StartMenu>();

    // 👇 絕對不能註解掉這行！必須切換到 UPDATE，main.cpp 才會開始畫圖！
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // 2. 因為我們現在的狀態是 UPDATE，所以直接執行選單的邏輯即可
    if (m_StartMenu) {
        m_StartMenu->Update(); // 這裡面包含了按鍵偵測與 Draw()
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