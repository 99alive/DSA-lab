#include <iostream>
#include <stdexcept>
#include "math_functions.h"

int main() {
    double num = 25.0;
    double result = compute_square_root(num);
    std::cout << "Square root of " << num << " is " << result << std::endl;

    // Negative inputs now report the error by throwing
    try {
        compute_square_root(-1.0);
    } catch (const std::domain_error& e) {
        std::cout << "Error for -1: " << e.what() << std::endl;
    }

    return 0;
}
