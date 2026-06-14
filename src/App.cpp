#include "App.hpp"
#include "Core/Context.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_BGM = std::make_shared<Util::BGM>("Resources/material/music/bg.mp3");
    m_BGM->Play(-1);
    m_StartMenu = std::make_shared<StartMenu>();
    m_LevelSelectMenu = nullptr;
    m_Gameplay = nullptr;
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if (m_BGM) {
        if (Util::Input::IsKeyDown(Util::Keycode::UP)) {
            m_Volume += 5;
            if (m_Volume > 128) m_Volume = 128;
            m_BGM->SetVolume(m_Volume);
            LOG_TRACE("Volume Up: {}", m_Volume);
        }
        if (Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
            m_Volume -= 5;
            if (m_Volume < 0) m_Volume = 0;
            m_BGM->SetVolume(m_Volume);
            LOG_TRACE("Volume Down: {}", m_Volume);
        }
    }

    if (m_StartMenu) {
        m_StartMenu->Update();
        if (m_StartMenu->IsGameStarted()) {
            m_LevelSelectMenu = std::make_shared<LevelSelectMenu>();
            m_StartMenu = nullptr;
        }
    }
    else if (m_LevelSelectMenu) {
        // ✨ 1. 如果遊戲還沒被創造出來 (還在正常選單階段)
        if (!m_Gameplay) {
            m_LevelSelectMenu->Update();
            m_LevelSelectMenu->Draw();

            if (m_LevelSelectMenu->IsLevelSelected()) {
                LevelType chosenLevel = m_LevelSelectMenu->GetSelectedLevel();
                m_Gameplay = std::make_shared<Gameplay>(chosenLevel);
                // ⚠️ 注意：我們在這裡「不」摧毀選單！留著它當轉場的底圖！
            }
            else if (m_LevelSelectMenu->WantsToReturn()) {
                m_StartMenu = std::make_shared<StartMenu>();
                m_LevelSelectMenu = nullptr;
            }
        }
        // ✨ 2. 如果選單和遊戲「同時存在」，代表正在播放關門轉場！
        else {
            m_LevelSelectMenu->Draw(); // 繼續畫出選單當背景
            m_Gameplay->Update();      // 更新遊戲 (此時遊戲只會負責畫出滑入的布幕)

            // ✨ 當布幕完全關上，把畫面遮住的瞬間，偷偷把選單摧毀！
            if (m_Gameplay->IsCurtainClosed()) {
                m_LevelSelectMenu = nullptr;
            }
        }
    }
    else if (m_Gameplay) {
        m_Gameplay->Update();
        if (m_Gameplay->WantsToReturnMenu()) {
            m_StartMenu = std::make_shared<StartMenu>();
            m_Gameplay = nullptr;
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { LOG_TRACE("End"); }