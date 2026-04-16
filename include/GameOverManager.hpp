#ifndef GAMEOVERMANAGER_HPP
#define GAMEOVERMANAGER_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp"
#include <vector>
#include <memory>

class GameOverManager {
public:
    GameOverManager();
    ~GameOverManager() = default;

    // 傳入所有水果，檢查是否有滿出來
    void CheckGameOver(const std::vector<std::shared_ptr<Fruit>>& fruits, int currentScore);
    // 繪製 Game Over UI
    void Draw();
    // 狀態 Getter
    bool IsGameOver() const { return m_IsGameOver; }
    // 是否按下R鍵準備回主選單
    bool IsReadyToReturnMenu() const;
private:
    bool m_IsGameOver = false;
    float m_DeathLineY = 240.0f; // 死亡線高度 (像素)
    int m_FramesAboveLine = 0;
    std::shared_ptr<Util::GameObject> m_GameOverText;
    std::shared_ptr<Util::GameObject> m_RestartText;
    std::shared_ptr<Util::GameObject> m_FinalScoreText;
    std::shared_ptr<Util::GameObject> m_Overlay;
};
#endif