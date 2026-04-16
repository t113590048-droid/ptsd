#ifndef UIManager_HPP
#define UIManager_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp"
#include <memory>
#include <string>

class UIManager {
public:
    UIManager();
    ~UIManager() = default;
    // 繪製所有的 UI 元件
    void Draw();
    // 更新分數顯示
    void UpdateScore(int score);
    // 更新下一個水果的預告顯示
    void UpdateNextFruit(FruitLevel level);
    // 更新當前待發射水果的顯示
    void UpdateCurrentFruit(FruitLevel level, float x);
private:
    // 背景與框架
    std::shared_ptr<Util::GameObject> m_Background;
    std::shared_ptr<Util::GameObject> m_Container;
    // 分數區
    std::shared_ptr<Util::GameObject> m_ScoreLabel;
    std::shared_ptr<Util::GameObject> m_ScoreBubble;
    std::shared_ptr<Util::GameObject> m_ScoreText;
    // 預告區
    std::shared_ptr<Util::GameObject> m_NextLabel;
    std::shared_ptr<Util::GameObject> m_NextBubble;
    std::shared_ptr<Util::GameObject> m_NextFruitUI;
    // 進化圖表
    std::shared_ptr<Util::GameObject> m_EvolutionLabel;
    std::shared_ptr<Util::GameObject> m_EvolutionCircle;
    // 按鍵提示
    std::shared_ptr<Util::GameObject> m_ControlsText;
};
#endif