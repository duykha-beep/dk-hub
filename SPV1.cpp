#include <chrono>
#include <thread>

#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("neon")

struct Touch {
    float x = 0.0f;
    float y = 0.0f;
};

constexpr int TARGET_FPS = 120;
constexpr float SENSITIVITY = 1.5f;
constexpr float SMOOTH = 0.2f;

Touch lastTouch{};
Touch smoothTouch{};

inline Touch ApplySensitivity(const Touch& cur, const Touch& last) {
    return {
        (cur.x - last.x) * SENSITIVITY,
        (cur.y - last.y) * SENSITIVITY
    };
}

inline Touch Smooth(const Touch& prev, const Touch& cur) {
    return {
        prev.x + (cur.x - prev.x) * SMOOTH,
        prev.y + (cur.y - prev.y) * SMOOTH
    };
}

void OnTouchInput(float x, float y) {
    Touch raw{x, y};
    Touch sensitive = ApplySensitivity(raw, lastTouch);
    smoothTouch = Smooth(smoothTouch, sensitive);
    lastTouch = raw;
}

void UpdateLogic() {
}

void RenderFrame() {
}

void GameLoop() {
    using clock = std::chrono::high_resolution_clock;
    const double frameTime = 1.0 / TARGET_FPS;

    while (true) {
        auto start = clock::now();

        UpdateLogic();
        RenderFrame();

        auto end = clock::now();
        std::chrono::duration<double> elapsed = end - start;

        double sleepTime = frameTime - elapsed.count();
        if (sleepTime > 0) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(sleepTime)
            );
        }
    }
}

int main() {
    GameLoop();
    return 0;
}