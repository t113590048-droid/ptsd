#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
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
    std::shared_ptr<Util::GameObject> m_NpcBoard;
    std::shared_ptr<Util::GameObject> m_NpcFace; // ✨ Mouth (嘴巴)
    std::shared_ptr<Util::GameObject> m_NpcBlush; // ✨ Blush (腮紅)
    std::shared_ptr<Util::GameObject> m_NpcEyes;
    std::shared_ptr<Util::Image> m_EyeOpenImage;
    std::shared_ptr<Util::Image> m_EyeClosedImage;

    float m_BlinkTimer = 0.0f;
    float m_BlinkDurationTimer = 0.0f;
    bool m_IsBlinking = false;

    float GetRandomBlinkInterval();

    std::shared_ptr<Util::GameObject> m_AimLine;
    std::shared_ptr<Util::GameObject> m_CurrentFruitUI;
    float m_MoveSpeed = 7.0f;
    float m_LeftLimit = -140.0f; // ✨ 調整為 -140.0f，配合對稱留空邊界 (-140 - 55 = -195)
    float m_RightLimit = 255.0f; // ✨ 調整為 255.0f，配合對稱留空邊界 (255 - 55 = 200)

    bool m_IsDropping = false;

    // 控制發射冷卻 (CD)
    float m_CooldownTime = 0.3f;  // 技能冷卻時間：1 秒
    float m_CooldownTimer = 0.0f; // 目前的倒數計時器
};

#endif