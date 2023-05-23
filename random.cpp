#include "random.h"

GraphicProject::Random::Random() : random_module_(1000) {
    auto now = std::chrono::high_resolution_clock::now();
    random_generator_.seed(now.time_since_epoch().count());
}

GraphicProject::Random &GraphicProject::Random::GetInstance() {
    static Random instance;
    return instance;
}
