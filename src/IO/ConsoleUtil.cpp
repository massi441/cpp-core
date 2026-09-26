#include "Core/IO/ConsoleUtil.h"

#include <iostream>
#include <string>

#include "Core/Util/ConvertUtil.h"
#include "Core/Util/MathHelpers.h"

namespace ml {

int64_t getInputNumber(int64_t lowerBoundIncl, int64_t upperBoundIncl) {
    while (true) {
        std::cout << "Enter a number between " << lowerBoundIncl << " and " << upperBoundIncl << ": ";

        std::string input;
        std::getline(std::cin, input);

        int64_t fallback = -1;
        int64_t inputNumber = ml::toInt64(input, fallback);

        if (mathl::inRangeIncl(inputNumber, lowerBoundIncl, upperBoundIncl)) {
            return inputNumber;
        }
    }
}

std::string getInputString(const std::string& inputMessage) {
    std::string input;

    while (input.empty()) {
        std::cout << inputMessage;
        std::getline(std::cin, input);
    }

    return input;
}

void printIfNotEmpty(const std::string& message) {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
}

}
