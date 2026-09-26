#pragma once

#include <cstdint>
#include <string>

namespace ml {

int64_t getInputNumber(int64_t lowerBoundIncl, int64_t upperBoundIncl);
std::string getInputString(const std::string& inputMessage);
void printIfNotEmpty(const std::string& message);

}
