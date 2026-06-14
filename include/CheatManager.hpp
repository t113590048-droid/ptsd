#ifndef CHEATMANAGER_HPP
#define CHEATMANAGER_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp" // 需要知道 FruitLevel
#include <memory>
#include <vector>

class CheatManager {
public:
    enum class CheatOption { SCORE, GAMEOVER, FRUIT };
    // ✨ 新增狀態機
    enum class MenuState { MAIN, FRUIT_SELECT, FRUIT_PLACE };

    CheatManager();
    ~CheatManager() = default;

    void Update();
    void Draw();

    bool IsOpen() const { return m_IsOpen; }
    void Close() { m_IsOpen = false; m_State = MenuState::MAIN; }

    bool ConsumeScoreCheat();
    bool ConsumeGameoverCheat();

    // ✨ 新增：確認是否有指定水果要生成，並回傳種類與座標
    bool ConsumeFruitSpawn(FruitLevel& outLevel, float& outX, float& outY);

private:
    void UpdateVisuals();
    void UpdateFruitSelectVisuals(); // 更新選水果介面的畫面

    bool m_IsOpen = false;
    MenuState m_State = MenuState::MAIN; // 目前處於哪個選單狀態
    CheatOption m_CurrentOption = CheatOption::SCORE;

    // 主選單文字
    std::shared_ptr<Util::GameObject> m_ScoreText;
    std::shared_ptr<Util::GameObject> m_GameoverText;
    std::shared_ptr<Util::GameObject> m_FruitText;

    // ✨ 第二階段：選水果介面
    int m_SelectedFruitIndex = 0; // 0 ~ 10
    std::vector<std::shared_ptr<Util::GameObject>> m_FruitIcons;

    // ✨ 第三階段：任意放置 (幽靈水果)
    std::shared_ptr<Util::GameObject> m_GhostFruit;
    float m_GhostX = 0.0f;
    float m_GhostY = 0.0f;

    // 信號
    bool m_ScoreCheatTriggered = false;
    bool m_GameoverCheatTriggered = false;
    bool m_SpawnRequested = false; // 是否要求生成水果
};

#endif // CHEATMANAGER_HPP