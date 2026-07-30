#pragma once
#include <memory>
#include <vector>
#include <functional>


enum class EStepType: int {
    STARTING = 1 << 0, // 1
    FAMILY   = 1 << 1, // 2
    SIBLING  = 1 << 2, // 4
    LAST     = 1 << 3  // 8
};

class EStep {
public:
    EStep(unsigned int id, EStepType type, std::function<bool()> function, bool dependent);
    ~EStep();

    unsigned int id;
    EStepType type;
    bool dependent{true};
    bool start();

private:
    bool finishedSuccessfully{false};
    std::function<bool()> func;
};

struct EStepManager {
    //Threading logic
    inline static unsigned int availableThreads;
    inline static bool multithread{false};

    //Step logic
    static void init();
    void add(EStepType type, std::function<bool()> function, bool dependent = true);
    void runAll();
    std::vector<std::unique_ptr<EStep>> steps;
};