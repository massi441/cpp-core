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

}
