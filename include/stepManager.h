#pragma once
#include <map>
#include <string>
#include <thread>
#include <optional>

enum class StepType : uint_fast8_t {
    INDEPENDENT,
    FAMILY,
    SIBLING,
    DEPENDENT,
    LAST
};

class EStep;

struct EStepManager {
    unsigned int threadCount{std::thread::hardware_concurrency()};

    inline static std::map<unsigned int, EStep*> steps;


    inline static std::optional<uint_fast8_t> lastStepId;
    inline static std::optional<uint_fast8_t> startingStepId;
    inline static uint_fast8_t pNextStepId{0};

    static void addStep(StepType type, const std::string &description = "");
};

class EStep {
public:
    explicit EStep(StepType type, std::string description = "");
    ~EStep();

    [[nodiscard]] unsigned int getId() const { return id; }
private:
    StepType type;
    uint_fast8_t id{0};
    uint8_t priority : 4 {0b0000};
    std::string description;
};