#include "StartMenu.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Text.hpp"
#include "ScoreManager.hpp"

StartMenu::StartMenu() {
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/gameplaybg.png"));
    m_Background->m_Transform.scale = {0.67f, 0.67f};
    m_Background->SetZIndex(0);

    m_Logo = std::make_shared<Util::GameObject>();
    m_Logo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/logo.png"));
    m_Logo->SetZIndex(1);
    m_Logo->m_Transform.translation = {0.0f, 180.0f}; // 稍微往上提
    m_Logo->m_Transform.scale = {0.8f, 0.8f};         // 加上縮放比例避免太大

    // --- 遊戲開始按鈕 ---
    m_BtnStart = std::make_shared<Util::GameObject>();
    m_BtnStart->SetZIndex(1);
    m_BtnStart->m_Transform.translation = {0.0f, 20.0f};
    m_BtnStart->m_Transform.scale = {0.5f, 0.5f};     // 寬高縮小一半

    // --- 我的分數按鈕 ---
    m_BtnScore = std::make_shared<Util::GameObject>();
    m_BtnScore->SetZIndex(1);
    m_BtnScore->m_Transform.translation = {0.0f, -100.0f}; // 拉開間距
    m_BtnScore->m_Transform.scale = {0.5f, 0.5f};          // 寬高縮小一半

    // --- 說明按鈕 ---
    m_BtnHowTo = std::make_shared<Util::GameObject>();
    m_BtnHowTo->SetZIndex(1);
    m_BtnHowTo->m_Transform.translation = {0.0f, -220.0f}; // 拉開間距
    m_BtnHowTo->m_Transform.scale = {0.5f, 0.5f};          // 寬高縮小一半

    // --- 初始化教學畫面物件 ---
    m_HowToPlayScreen = std::make_shared<Util::GameObject>();
    m_HowToPlayScreen->SetZIndex(10);
    m_HowToPlayScreen->m_Transform.translation = {0.0f, 0.0f};
    // 如果圖檔太大修改倍率：
    m_HowToPlayScreen->m_Transform.scale = {0.666f, 0.666f};

    // --- 初始化計分表 UI 物件 ---

    m_ScoreboardTitle = std::make_shared<Util::GameObject>();
    m_ScoreboardTitle->SetZIndex(16);
    m_ScoreboardTitle->m_Transform.translation = {0.0f, 160.0f};

    m_ScoreboardReturnPrompt = std::make_shared<Util::GameObject>();
    m_ScoreboardReturnPrompt->SetZIndex(16);
    m_ScoreboardReturnPrompt->m_Transform.translation = {0.0f, -190.0f};

    UpdateButtonVisuals();
}

void StartMenu::Update() {
    // ==========================================
    // 狀態 1：如果在「計分表」中
    // ==========================================
    if (m_IsShowingScoreboard) {
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_IsShowingScoreboard = false;
        }
    }
    // ==========================================
    // 狀態 2：如果在「教學畫面」中
    // ==========================================
    else if (m_IsShowingHowToPlay) {
        if (Util::Input::IsKeyUp(Util::Keycode::D)) {
            if (m_HowToPlayPage < 3) {
                m_HowToPlayPage++;
                UpdateHowToPlayVisuals();
            }
        }
        if (Util::Input::IsKeyUp(Util::Keycode::A)) {
            if (m_HowToPlayPage > 1) {
                m_HowToPlayPage--;
                UpdateHowToPlayVisuals();
            }
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_IsShowingHowToPlay = false;
        }
    }
    // ==========================================
    // 狀態 3：如果在「主選單」中
    // ==========================================
    else {
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::GAME_START;
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
                m_CurrentOption = MenuOption::MY_SCORE;
            }
            UpdateButtonVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                m_CurrentOption = MenuOption::MY_SCORE;
            } else if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_CurrentOption = MenuOption::HOW_TO_PLAY;
            }
            UpdateButtonVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_CurrentOption == MenuOption::GAME_START) {
                LOG_TRACE("Game Started!");
                m_IsGameStarted = true;
            } else if (m_CurrentOption == MenuOption::MY_SCORE) {
                m_IsShowingScoreboard = true;
                UpdateScoreboardVisuals();
            } else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
                m_IsShowingHowToPlay = true;
                m_HowToPlayPage = 1;
                UpdateHowToPlayVisuals();
            }
        }
    }

    // --- 繪製畫面 ---
    if (m_Background) m_Background->Draw();
    if (!m_IsShowingHowToPlay && !m_IsShowingScoreboard) {
        if (m_Logo) m_Logo->Draw();
        if (m_BtnStart) m_BtnStart->Draw();
        if (m_BtnScore) m_BtnScore->Draw();
        if (m_BtnHowTo) m_BtnHowTo->Draw();
    }

    if (m_IsShowingHowToPlay && m_HowToPlayScreen) {
        m_HowToPlayScreen->Draw();
    }

    if (m_IsShowingScoreboard) {
        if (m_ScoreboardTitle) m_ScoreboardTitle->Draw();
        for (auto& line : m_ScoreboardLines) {
            if (line) line->Draw();
        }
        if (m_ScoreboardReturnPrompt) m_ScoreboardReturnPrompt->Draw();
    }
}

// ==========================================
// ★ 注意：如果你已經把圖片替換成了官方版
// 記得將這邊的檔名改成新的檔名！
// 例如將 howtoplaybutton1.png 改成 btn_htp.png
// ==========================================
void StartMenu::UpdateButtonVisuals() {
    if (m_CurrentOption == MenuOption::GAME_START) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton2.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton1.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton1.png"));
    }
    else if (m_CurrentOption == MenuOption::MY_SCORE) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton1.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton2.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton1.png"));
    }
    else if (m_CurrentOption == MenuOption::HOW_TO_PLAY) {
        m_BtnStart->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/startbutton1.png"));
        m_BtnScore->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/myscorebutton1.png"));
        m_BtnHowTo->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/button/howtoplaybutton2.png"));
    }
}

void StartMenu::UpdateHowToPlayVisuals() {
    if (m_HowToPlayPage == 1) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay1.png"));
    } else if (m_HowToPlayPage == 2) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay2.png"));
    } else if (m_HowToPlayPage == 3) {
        m_HowToPlayScreen->SetDrawable(std::make_shared<Util::Image>("Resources/material/first page/howtoplay/howtoplay3.png"));
    }
}

void StartMenu::UpdateScoreboardVisuals() {
    ScoreManager::Stats stats = ScoreManager::Load();

    // 1. 設定標題
    m_ScoreboardTitle->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/FOT-OEDKTRSTD-E.otf",
        32,
        "★ 個人排行榜 & 遊戲統計 ★",
        Util::Color(80, 50, 20, 255)
    ));

    // 2. 清空舊的文字列
    m_ScoreboardLines.clear();

    // 3. 填入前 5 名分數
    float yOffset = 90.0f;
    for (int i = 0; i < 5; ++i) {
        auto scoreLine = std::make_shared<Util::GameObject>();
        scoreLine->SetZIndex(16);
        scoreLine->m_Transform.translation = {0.0f, yOffset};
        
        std::string rankStr = "第 " + std::to_string(i + 1) + " 名 (Rank " + std::to_string(i + 1) + ") :  " + std::to_string(stats.topScores[i]) + " 分";
        scoreLine->SetDrawable(std::make_shared<Util::Text>(
            "Resources/material/font/FOT-OEDKTRSTD-E.otf",
            22,
            rankStr,
            Util::Color(100, 70, 40, 255)
        ));
        m_ScoreboardLines.push_back(scoreLine);
        yOffset -= 30.0f;
    }

    // 4. 填入統計資料
    const std::string FRUIT_NAMES[] = {
        "櫻桃 (Cherry)",
        "草莓 (Strawberry)",
        "葡萄 (Grape)",
        "柑橘 (Tangerine)",
        "柿子 (Orange)",
        "蘋果 (Apple)",
        "梨子 (Pear)",
        "水蜜桃 (Peach)",
        "鳳梨 (Pineapple)",
        "哈密瓜 (Melon)",
        "大西瓜 (Watermelon)"
    };
    std::string fruitName = "未知";
    if (stats.maxFruitLevel >= 1 && stats.maxFruitLevel <= 11) {
        fruitName = FRUIT_NAMES[stats.maxFruitLevel - 1];
    }

    yOffset = -80.0f;
    
    // 總遊玩次數
    {
        auto line = std::make_shared<Util::GameObject>();
        line->SetZIndex(16);
        line->m_Transform.translation = {0.0f, yOffset};
        line->SetDrawable(std::make_shared<Util::Text>(
            "Resources/material/font/FOT-OEDKTRSTD-E.otf",
            20,
            "總遊玩次數 (Total Games) : " + std::to_string(stats.totalGames) + " 次",
            Util::Color(70, 70, 70, 255)
        ));
        m_ScoreboardLines.push_back(line);
        yOffset -= 28.0f;
    }

    // 最大合成水果
    {
        auto line = std::make_shared<Util::GameObject>();
        line->SetZIndex(16);
        line->m_Transform.translation = {0.0f, yOffset};
        line->SetDrawable(std::make_shared<Util::Text>(
            "Resources/material/font/FOT-OEDKTRSTD-E.otf",
            20,
            "最大合成水果 (Max Fruit) : " + fruitName,
            Util::Color(70, 70, 70, 255)
        ));
        m_ScoreboardLines.push_back(line);
        yOffset -= 28.0f;
    }

    // 累積總分數
    {
        auto line = std::make_shared<Util::GameObject>();
        line->SetZIndex(16);
        line->m_Transform.translation = {0.0f, yOffset};
        line->SetDrawable(std::make_shared<Util::Text>(
            "Resources/material/font/FOT-OEDKTRSTD-E.otf",
            20,
            "累積總得分 (Total Score) : " + std::to_string(stats.totalAccumulatedScore) + " 分",
            Util::Color(70, 70, 70, 255)
        ));
        m_ScoreboardLines.push_back(line);
    }

    // 5. 設定返回提示
    m_ScoreboardReturnPrompt->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/FOT-OEDKTRSTD-E.otf",
        20,
        "[ 按空白鍵 Space 返回主選單 ]",
        Util::Color(120, 120, 120, 255)
    ));
}