#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>

namespace GraphicProject {

class Random {
private:
    Random();

public:
    Random(const Random &other) = delete;
    Random &operator=(const Random &other) = delete;

public:
    static Random &GetInstance();

    int GetInt() { return random_generator_() % random_module_; }

    void SetModule(int module) { random_module_ = module; }

private:
    std::mt19937 random_generator_;
    int random_module_;
};

} // namespace GraphicProject

#endif // RANDOM_H
