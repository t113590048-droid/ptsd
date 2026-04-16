#ifndef PAUSEMANAGER_HPP
#define PAUSEMANAGER_HPP

#include "Util/GameObject.hpp"
#include <memory>

class PauseManager {
public:
    PauseManager();
    ~PauseManager() = default;
    // 處理按鍵偵測
    void Update();
    // 繪製暫停畫面的文字
    void Draw();
    // 狀態 Getter
    bool IsPaused() const { return m_IsPaused; }
    bool IsReadyToReturnMenu() const;
private:
    bool m_IsPaused = false;
    std::shared_ptr<Util::GameObject> m_PauseTitle;
    std::shared_ptr<Util::GameObject> m_ResumeText;
    std::shared_ptr<Util::GameObject> m_RestartText;
};
#endif // PAUSEMANAGER_HPP