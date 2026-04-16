#include "Cloud.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <string>

Cloud::Cloud() {
    // 1. 初始化雲朵
    m_CloudImg = std::make_shared<Util::GameObject>();
    m_CloudImg->SetDrawable(std::make_shared<Util::Image>("Resources/material/cloud.png"));
    m_CloudImg->SetZIndex(10);
    m_CloudImg->m_Transform.translation = {48.0f, 310.0f};
    m_CloudImg->m_Transform.scale = {1.5f, 1.5f};
    // 2. 初始化瞄準線
    m_AimLine = std::make_shared<Util::GameObject>();
    m_AimLine->SetDrawable(std::make_shared<Util::Image>("Resources/material/aimline.png"));
    m_AimLine->SetZIndex(2);
    m_AimLine->m_Transform.translation = {48.0f, 20.0f};
    m_AimLine->m_Transform.scale = {1.0f, 1.95f};
    // 3. 初始化手上的水果空殼 (圖片會在 SetCurrentFruit 給予)
    m_CurrentFruitUI = std::make_shared<Util::GameObject>();
    m_CurrentFruitUI->SetZIndex(11);
}
void Cloud::SetCurrentFruit(FruitLevel level) {
    std::string path = "Resources/material/fruit/";
    float scale = 0.6f;
    switch (level) {
        case FruitLevel::Cherry: path += "cherry.png"; break;
        case FruitLevel::Strawberry: path += "strawberry.png"; break;
        case FruitLevel::Grape: path += "grape.png"; break;
        case FruitLevel::tangerine: path += "tangerine.png"; break;
        case FruitLevel::Orange: path += "orange.png"; break;
        case FruitLevel::Apple: path += "apple.png"; break;
        case FruitLevel::Pear: path += "pear.png"; break;
        case FruitLevel::Peach: path += "peach.png"; break;
        case FruitLevel::Pineapple: path += "pineapple.png"; break;
        case FruitLevel::Melon: path += "melon.png"; break;
        case FruitLevel::Watermelon: path += "watermelon.png"; break;
        default: path += "cherry.png"; break;
    }
    m_CurrentFruitUI->SetDrawable(std::make_shared<Util::Image>(path));
    m_CurrentFruitUI->m_Transform.scale = {scale, scale};
}
void Cloud::Update() {
    // 1. 偵測左右移動
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) m_CloudImg->m_Transform.translation.x -= m_MoveSpeed;
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) m_CloudImg->m_Transform.translation.x += m_MoveSpeed;
    // 2. 限制不出界
    if (m_CloudImg->m_Transform.translation.x > m_RightLimit) m_CloudImg->m_Transform.translation.x = m_RightLimit;
    if (m_CloudImg->m_Transform.translation.x < m_LeftLimit) m_CloudImg->m_Transform.translation.x = m_LeftLimit;
    // 3. 讓瞄準線與手上的水果跟著雲朵走
    if (m_AimLine) m_AimLine->m_Transform.translation.x = m_CloudImg->m_Transform.translation.x - 45.0f;
    if (m_CurrentFruitUI) {
        m_CurrentFruitUI->m_Transform.translation.x = m_CloudImg->m_Transform.translation.x - 45.0f;
        m_CurrentFruitUI->m_Transform.translation.y = 280.0f;
    }
    // 4. 減少冷卻計時器
    // 遊戲迴圈大約是 60 FPS，所以每一幀減去 1/60 秒
    if (m_CooldownTimer > 0.0f) {
        m_CooldownTimer -= 1.0f / 60.0f;
    }
    // 5. 偵測發射鍵 (SPACE)，並且確認 CD 已經好了 (m_CooldownTimer <= 0)
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) && m_CooldownTimer <= 0.0f) {
        m_IsDropping = true;
        m_CooldownTimer = m_CooldownTime; // 重置冷卻時間，開始重新倒數
    } else {
        // 如果沒按空白鍵，或者是 CD 還沒好，就不允許發射
        m_IsDropping = false;
    }
}
bool Cloud::IsDroppingFruit() const {
    return m_IsDropping;
}
float Cloud::GetDropX() const {
    // 水果生成的 X 座標就是假水果的 X 座標
    return m_CurrentFruitUI->m_Transform.translation.x;
}
void Cloud::Draw() {
    if (m_AimLine) m_AimLine->Draw();
    if (m_CurrentFruitUI) m_CurrentFruitUI->Draw();
    if (m_CloudImg) m_CloudImg->Draw();
}