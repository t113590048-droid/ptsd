#ifndef LEVELTYPE_HPP
#define LEVELTYPE_HPP

enum class LevelType {
    NORMAL,            // 0: 一般模式
    STATIC_PEGS,       // 1: 靜態釘柱
    MOVING_PLATFORMS,  // 2: 移動平台
    PORTAL             // 3: 傳送門模式
};

#endif // LEVELTYPE_HPP