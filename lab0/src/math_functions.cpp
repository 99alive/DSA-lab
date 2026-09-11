#include "math_functions.h"
#include <cmath>      // For std::sqrt
#include <stdexcept>  // For std::domain_error

double compute_square_root(double number) {
    if (number < 0) {
        // Negative inputs are outside the domain of the real square root
        throw std::domain_error("compute_square_root: negative input has no real square root");
    }
    return std::sqrt(number);
}
