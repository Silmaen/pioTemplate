/**
 * @file functions.h
 * @author argawaen
 * @date 11/07/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#ifdef ARDUINO_ARCH_AVR
#include "math.h"
#else
#include <cmath>
#endif

namespace sbs::math {

/**
 * @brief Logarithm
 * @tparam BaseType Value and return type
 * @param value Value to compute
 * @return Logarithm of value
 */
template<class BaseType>
constexpr inline BaseType log(const BaseType& value) {
#ifdef ARDUINO_ARCH_AVR
    return ::log(value);
#else
    return std::log(value);
#endif
}

/**
 * @brief Exponential
 * @tparam BaseType Value and return type
 * @param value Value to compute
 * @return Exponential of value
 */
template<class BaseType>
constexpr inline BaseType exp(const BaseType& value) {
#ifdef ARDUINO_ARCH_AVR
    return ::exp(value);
#else
    return std::exp(value);
#endif
}

}// namespace sbs::math
