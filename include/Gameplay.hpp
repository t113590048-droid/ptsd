#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "Util/GameObject.hpp"
#include <memory>

class Gameplay {
public:
    Gameplay(); // 建構子：負責載入素材與設定位置
    void Update(); // 每一幀呼叫：負責繪製

private:
    // --- 靜態背景物件 ---
    std::shared_ptr<Util::GameObject> m_Background; // 整體背景
    std::shared_ptr<Util::GameObject> m_Container;  // 水果容器盒

    // --- UI 元件 ---
    std::shared_ptr<Util::GameObject> m_ScoreLabel;      // "SCORE" 文字圖片
    std::shared_ptr<Util::GameObject> m_NextLabel;       // "NEXT" 文字圖片
    std::shared_ptr<Util::GameObject> m_NextBubble;      // "NEXT" 下方泡泡框
    std::shared_ptr<Util::GameObject> m_EvolutionLabel; // "Circle of evolution" 文字
    std::shared_ptr<Util::GameObject> m_EvolutionCircle; // 水果進化圈圖
    std::shared_ptr<Util::GameObject> m_AimLine;

    // --- 準備區域 ---
    std::shared_ptr<Util::GameObject> m_Cloud; // 上方的雲 (水果準備區)
};

#endif // GAMEPLAY_HPP