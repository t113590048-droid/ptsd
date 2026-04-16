#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "Util/GameObject.hpp"
#include "Fruit.hpp"
#include <memory>

class Cloud {
public:
    Cloud();
    ~Cloud() = default;
    // 處理玩家輸入與雲朵移動
    void Update();
    // 繪製雲朵、瞄準線與手上的水果
    void Draw();
    // 更新雲朵手上拿的水果圖案
    void SetCurrentFruit(FruitLevel level);
    // 讓 Gameplay 知道玩家是否按下了發射鍵 (SPACE)
    bool IsDroppingFruit() const;
    // 取得要生成水果的 X 座標
    float GetDropX() const;
private:
    std::shared_ptr<Util::GameObject> m_CloudImg;
    std::shared_ptr<Util::GameObject> m_AimLine;
    std::shared_ptr<Util::GameObject> m_CurrentFruitUI;
    float m_MoveSpeed = 7.0f;
    float m_LeftLimit = -175.0f;
    float m_RightLimit = 245.0f;

    bool m_IsDropping = false;

    // 控制發射冷卻 (CD)
    float m_CooldownTime = 0.3f;  // 技能冷卻時間：1 秒
    float m_CooldownTimer = 0.0f; // 目前的倒數計時器
};

#endif