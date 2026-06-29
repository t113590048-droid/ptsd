#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class ScoreManager {
public:
    struct Stats {
        std::vector<int> topScores = {0, 0, 0, 0, 0};
        int totalGames = 0;
        int maxFruitLevel = 1; // 1 = Cherry, 11 = Watermelon
        int totalAccumulatedScore = 0;
    };

    static Stats Load() {
        Stats stats;
        std::ifstream file("scores.txt");
        if (!file.is_open()) {
            return stats;
        }
        
        for (int i = 0; i < 5; ++i) {
            if (!(file >> stats.topScores[i])) {
                stats.topScores[i] = 0;
            }
        }
        if (!(file >> stats.totalGames)) stats.totalGames = 0;
        if (!(file >> stats.maxFruitLevel)) stats.maxFruitLevel = 1;
        if (!(file >> stats.totalAccumulatedScore)) stats.totalAccumulatedScore = 0;
        
        file.close();
        return stats;
    }

    static void Save(const Stats& stats) {
        std::ofstream file("scores.txt");
        if (!file.is_open()) return;
        
        for (int i = 0; i < 5; ++i) {
            file << stats.topScores[i] << "\n";
        }
        file << stats.totalGames << "\n";
        file << stats.maxFruitLevel << "\n";
        file << stats.totalAccumulatedScore << "\n";
        file.close();
    }

    static void RecordGame(int score, int maxFruit) {
        Stats stats = Load();
        stats.totalGames++;
        stats.totalAccumulatedScore += score;
        if (maxFruit > stats.maxFruitLevel) {
            stats.maxFruitLevel = maxFruit;
        }
        stats.topScores.push_back(score);
        std::sort(stats.topScores.rbegin(), stats.topScores.rend());
        stats.topScores.resize(5);
        Save(stats);
    }
};

#endif // SCORE_MANAGER_HPP
