#ifndef OPENINGANIMATION_HPP
#define OPENINGANIMATION_HPP

#include "Util/GameObject.hpp"
#include <vector>
#include <memory>

class OpeningAnimation {
public:
    enum class State { CURTAIN_IN, CURTAIN_ANIM, READY, GO, CURTAIN_OUT, FINISHED };

    OpeningAnimation();
    ~OpeningAnimation() = default;

    void Update(float deltaTime);
    void Draw();

    bool IsFinished() const { return m_State == State::FINISHED; }
    bool IsCurtainClosed() const { return m_Timer > 1.0f; }
    bool IsBlocking() const { return m_Timer < 4.0f; }

private:
    State m_State = State::CURTAIN_IN;
    float m_Timer = 0.0f;
    float m_RotationAngle = 0.0f;

    std::shared_ptr<Util::GameObject> m_ReadyText;
    std::shared_ptr<Util::GameObject> m_GoText;
    std::vector<std::shared_ptr<Util::GameObject>> m_BubbleFruits;

    // ✨ 新增：左右兩邊的布幕
    std::shared_ptr<Util::GameObject> m_LeftCurtain;
    std::shared_ptr<Util::GameObject> m_RightCurtain;
    int m_CurtainFrame = 5;
    float m_CurtainAnimTimer = 0.0f;
};

#endif // OPENINGANIMATION_HPP