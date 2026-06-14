#include "CheatManager.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"

CheatManager::CheatManager() {
    // === 1. 初始化主選單文字 ===
    m_ScoreText = std::make_shared<Util::GameObject>();
    m_ScoreText->SetZIndex(100);
    m_ScoreText->m_Transform.translation = {0.0f, 50.0f};

    m_GameoverText = std::make_shared<Util::GameObject>();
    m_GameoverText->SetZIndex(100);
    m_GameoverText->m_Transform.translation = {0.0f, 0.0f};

    m_FruitText = std::make_shared<Util::GameObject>();
    m_FruitText->SetZIndex(100);
    m_FruitText->m_Transform.translation = {0.0f, -50.0f};

    // === 2. 初始化 11 顆水果的選擇清單 ===
    std::vector<std::string> fruitNames = {
        "cherry", "strawberry", "grape", "tangerine", "orange",
        "apple", "pear", "peach", "pineapple", "melon", "watermelon"
    };
    for (int i = 0; i < 11; i++) {
        auto icon = std::make_shared<Util::GameObject>();
        icon->SetDrawable(std::make_shared<Util::Image>("Resources/material/fruit/" + fruitNames[i] + ".png"));
        icon->SetZIndex(100);
        if (i < 7) {
            // 上排 7 顆 (較小)：起點 -300，間距 100，高度在 Y=100
            icon->m_Transform.translation = {-350.0f + (i * (0.05 * i + 1) * 80.0f), 100.0f};
        } else {
            // 下排 4 顆 (較大)：起點 -270，間距 180，高度在 Y=-100
            int bottomIndex = i - 7; // 讓下排的索引從 0 開始算 (0, 1, 2, 3)
            icon->m_Transform.translation = {-400.0f + (bottomIndex * (0.05 * bottomIndex + 1) * 220.0f), -100.0f};
        }
        m_FruitIcons.push_back(icon);
    }

    // === 3. 初始化幽靈水果 (用來隨意移動) ===
    m_GhostFruit = std::make_shared<Util::GameObject>();
    m_GhostFruit->SetZIndex(100);

    UpdateVisuals();
}

void CheatManager::Update() {
    // 偵測 C 鍵開關選單 (任何狀態按 C 都可以直接關閉)
    if (Util::Input::IsKeyUp(Util::Keycode::C)) {
        m_IsOpen = !m_IsOpen;
        if (m_IsOpen) {
            m_State = MenuState::MAIN; // 重新打開時回到主選單
            m_CurrentOption = CheatOption::SCORE;
            UpdateVisuals();
        }
    }

    if (!m_IsOpen) return;

    // ==========================================
    // 狀態機處理：依照不同階段處理不同的按鍵
    // ==========================================
    if (m_State == MenuState::MAIN) {
        if (Util::Input::IsKeyUp(Util::Keycode::W)) {
            if (m_CurrentOption == CheatOption::GAMEOVER) m_CurrentOption = CheatOption::SCORE;
            else if (m_CurrentOption == CheatOption::FRUIT) m_CurrentOption = CheatOption::GAMEOVER;
            UpdateVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::S)) {
            if (m_CurrentOption == CheatOption::SCORE) m_CurrentOption = CheatOption::GAMEOVER;
            else if (m_CurrentOption == CheatOption::GAMEOVER) m_CurrentOption = CheatOption::FRUIT;
            UpdateVisuals();
        }

        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            if (m_CurrentOption == CheatOption::SCORE) m_ScoreCheatTriggered = true;
            else if (m_CurrentOption == CheatOption::GAMEOVER) m_GameoverCheatTriggered = true;
            else if (m_CurrentOption == CheatOption::FRUIT) {
                // ✨ 進入選水果階段
                m_State = MenuState::FRUIT_SELECT;
                m_SelectedFruitIndex = 0;
                UpdateFruitSelectVisuals();
            }
            return; // 提早結束，避免 SPACE 鍵連續觸發
        }
    }
    else if (m_State == MenuState::FRUIT_SELECT) {
        // A / D 控制左右選擇水果
        if (Util::Input::IsKeyUp(Util::Keycode::A) && m_SelectedFruitIndex > 0) {
            m_SelectedFruitIndex--;
            UpdateFruitSelectVisuals();
        }
        if (Util::Input::IsKeyUp(Util::Keycode::D) && m_SelectedFruitIndex < 10) {
            m_SelectedFruitIndex++;
            UpdateFruitSelectVisuals();
        }

        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            // ✨ 選定水果，進入自由放置階段
            m_State = MenuState::FRUIT_PLACE;

            // 既然不能 GetDrawable，我們直接重新讀取對應的圖片！
            std::vector<std::string> fruitNames = {
                "cherry", "strawberry", "grape", "tangerine", "orange",
                "apple", "pear", "peach", "pineapple", "melon", "watermelon"
            };
            std::string path = "Resources/material/fruit/" + fruitNames[m_SelectedFruitIndex] + ".png";
            m_GhostFruit->SetDrawable(std::make_shared<Util::Image>(path));
            m_GhostFruit->m_Transform.scale = {0.6f, 0.6f};

            m_GhostX = 0.0f; // 預設出現在雲的下方中央
            m_GhostY = 200.0f;
            return;
        }
    }
    else if (m_State == MenuState::FRUIT_PLACE) {
        // W A S D 自由控制座標
        float speed = 10.0f;
        if (Util::Input::IsKeyPressed(Util::Keycode::W)) m_GhostY += speed;
        if (Util::Input::IsKeyPressed(Util::Keycode::S)) m_GhostY -= speed;
        if (Util::Input::IsKeyPressed(Util::Keycode::A)) m_GhostX -= speed;
        if (Util::Input::IsKeyPressed(Util::Keycode::D)) m_GhostX += speed;

        // ==========================================
        // ✨ 新增：容器邊界限制 (隱形牆)
        // 這些數字是假設的容器大小，請根據你遊戲畫面中實際的玻璃罐寬度微調！
        // ==========================================
        float minX = -180.0f; // 左邊界
        float maxX =  180.0f; // 右邊界
        float minY = -250.0f; // 下邊界 (地板)
        float maxY =  300.0f; // 上邊界 (雲的高度或更高一點)

        // 強制把座標拉回範圍內
        if (m_GhostX < minX) m_GhostX = minX;
        if (m_GhostX > maxX) m_GhostX = maxX;
        if (m_GhostY < minY) m_GhostY = minY;
        if (m_GhostY > maxY) m_GhostY = maxY;

        // 將算好的座標套用到幽靈水果上
        m_GhostFruit->m_Transform.translation = {m_GhostX, m_GhostY};

        // 按 SPACE 確定召喚！
        if (Util::Input::IsKeyUp(Util::Keycode::SPACE)) {
            m_SpawnRequested = true;
            Close();
            return;
        }
    }
}

void CheatManager::UpdateFruitSelectVisuals() {
    std::vector<std::string> fruitNames = {
        "cherry", "strawberry", "grape", "tangerine", "orange",
        "apple", "pear", "peach", "pineapple", "melon", "watermelon"
    };

    for (int i = 0; i < 11; i++) {
        std::string suffix = (i == m_SelectedFruitIndex) ? "_sad.png" : ".png";
        std::string path = "Resources/material/fruit/" + fruitNames[i] + suffix;

        m_FruitIcons[i]->SetDrawable(std::make_shared<Util::Image>(path));
        m_FruitIcons[i]->m_Transform.scale = {0.6f, 0.6f};

        // ✨ 關鍵修正：切換圖片時，要保持它們各自在正確的排數高度！
        if (i < 7) {
            m_FruitIcons[i]->m_Transform.translation.y = 100.0f;  // 上排
        } else {
            m_FruitIcons[i]->m_Transform.translation.y = -100.0f; // 下排
        }
    }
}
void CheatManager::UpdateVisuals() {
    // 未選中的顏色 (灰色)
    Util::Color unselectedColor(150, 150, 150, 255);
    // 選中的顏色 (黃色)
    Util::Color selectedColor(255, 255, 0, 255);

    // 更新 Score 文字
    bool isScore = (m_CurrentOption == CheatOption::SCORE);
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf", 40,
        isScore ? "> Score <" : "Score",
        isScore ? selectedColor : unselectedColor
    ));

    // 更新 Gameover 文字
    bool isGameover = (m_CurrentOption == CheatOption::GAMEOVER);
    m_GameoverText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf", 40,
        isGameover ? "> Gameover <" : "Gameover",
        isGameover ? selectedColor : unselectedColor
    ));

    // 更新 Fruit 文字
    bool isFruit = (m_CurrentOption == CheatOption::FRUIT);
    m_FruitText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf", 40,
        isFruit ? "> Fruit <" : "Fruit",
        isFruit ? selectedColor : unselectedColor
    ));
}
bool CheatManager::ConsumeFruitSpawn(FruitLevel& outLevel, float& outX, float& outY) {
    if (m_SpawnRequested) {
        m_SpawnRequested = false;
        outLevel = static_cast<FruitLevel>(m_SelectedFruitIndex);
        outX = m_GhostX;
        outY = m_GhostY;
        return true;
    }
    return false;
}

void CheatManager::Draw() {
    if (!m_IsOpen) return;

    if (m_State == MenuState::MAIN) {
        if (m_ScoreText) m_ScoreText->Draw();
        if (m_GameoverText) m_GameoverText->Draw();
        if (m_FruitText) m_FruitText->Draw();
    }
    else if (m_State == MenuState::FRUIT_SELECT) {
        for (int i = 0; i < 11; i++) {
            if (i == m_SelectedFruitIndex) {
                // ✨ 抖動特效：先記住原本的位置
                auto originalPos = m_FruitIcons[i]->m_Transform.translation;

                // 加上隨機偏移量 (-3.0 ~ 3.0)
                m_FruitIcons[i]->m_Transform.translation.x += ((std::rand() % 7) - 3) * 1.0f;
                m_FruitIcons[i]->m_Transform.translation.y += ((std::rand() % 7) - 3) * 1.0f;

                m_FruitIcons[i]->Draw(); // 用發抖的位置畫出來

                // ✨ 畫完立刻把位置復原！(避免水果飄走)
                m_FruitIcons[i]->m_Transform.translation = originalPos;
            } else {
                // 沒選到的就乖乖正常畫出來
                m_FruitIcons[i]->Draw();
            }
        }
    }
    else if (m_State == MenuState::FRUIT_PLACE) {
        if (m_GhostFruit) m_GhostFruit->Draw();
    }
}
bool CheatManager::ConsumeScoreCheat() {
    // 如果有觸發作弊，就把信號「消耗」掉（歸零），並回傳 true 給 Gameplay
    if (m_ScoreCheatTriggered) {
        m_ScoreCheatTriggered = false;
        return true;
    }
    return false;
}
bool CheatManager::ConsumeGameoverCheat() {
    if (m_GameoverCheatTriggered) {
        m_GameoverCheatTriggered = false;
        return true;
    }
    return false;
}