/**
 * @file Print.cpp
 * @author Silmaen
 * @date 12/07/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Print.h"
#include "math/functions.h"
#ifdef NATIVE
#include <cmath>
#include <iostream>
#else
#include <Arduino.h>
#endif
namespace sbs::io {
/**
 * @brief Global verbosity level
 */
static Verbosity verbose = Verbosity::Error;

/**
 * @brief Internal function to print line prefix
 * @param verbosity Message's verbosity level
 * @return If message should be print
 */
bool printPrefix(const Verbosity& verbosity) {
    if (verbose == Verbosity::Mute) return false;
    if (verbosity == Verbosity::Error) {
#ifdef NATIVE
        std::cout << "ERROR ";
#else
        Serial.print(F("ERROR "));
#endif
        return true;
    }
    if (verbosity == Verbosity::Warning && verbose != Verbosity::Error) {
#ifdef NATIVE
        std::cout << "WARNING ";
#else
        Serial.print(F("WARNING "));
#endif
        return true;
    }
    if (verbosity == Verbosity::Debug && verbose == Verbosity::Debug) {
#ifdef NATIVE
        std::cout << "DEBUG ";
#else
        Serial.print(F("DEBUG "));
#endif
        return true;
    }
    return verbosity == Verbosity::Mute;
}

void setVerbosity(const Verbosity& verb) {
    verbose = verb;
}

/**
 * @brief Internal function to print some chars
 * @param str The basic string to print
 * @param verbosity Message's verbosity level
 */
void print(const char* str, const Verbosity& verbosity) {
    if (!printPrefix(verbosity)) return;
#ifdef NATIVE
    std::cout << str;
#else
    Serial.print(str);
#endif
}

/**
 * @brief Internal function to print string
 * @param str String to print
 * @param verbosity Message's verbosity level
 */
void print(const string& str, const Verbosity& verbosity) {
    print(str.c_str(), verbosity);
}

/**
 * @brief Print a single char
 * @param c The char to print
 */
void printChar(char c){
#ifdef NATIVE
    std::cout << c;
#else
    Serial.print(c);
#endif
}

/**
 * @brief Print integer as bits
 * @tparam T The integer type
 * @param data The Integer to print
 */
template<class T>
void toBits(const T& data) {
    uint8_t bits    = sizeof(T) * 8;
    uint64_t cursor = 1ULL << (bits - 1);
    while (cursor != 0) {
        printChar((data & cursor) ? '1' : '0');
        cursor >>= 1;
    }
}
/// List of hexadecimal digits
const char digits[] = "0123456789ABCDEF";

/**
 * @brief Print integer in Hex format
 * @tparam T The integer type
 * @param data The integer to print
 */
template<class T>
void toHex(const T& data) {
    uint8_t octets  = sizeof(T) * 2;
    uint8_t ggg = octets-1;
    uint64_t cursor = 15ULL << (ggg* 4);
    while (cursor != 0) {
        printChar(digits[(data & cursor)>>(4*ggg)]);
        cursor >>= 4;
        --ggg;
    }
}

/**
 * @brief Internal function for printing integer
 * @tparam BaseType Integer Type
 * @param data The integer to print
 * @param format The integer format
 * @param verbosity The message's verbosity
 */
template<class BaseType>
void print(BaseType data, const IntFormat& format, const Verbosity& verbosity) {
    if (!printPrefix(verbosity)) return;
    if (format == IntFormat::Binary) {
        toBits(data);
        return;
    }
    if (format == IntFormat::Hexadecimal) {
        toHex(data);
        return;
    }
#ifdef NATIVE
    std::cout << static_cast<int32_t>(data);
#else
    Serial.print(static_cast<int32_t>(data));
#endif
}

/**
 * @brief Internal function to print double
 * @param data The double to print
 * @param digit The number of digits
 * @param verbosity The Message's verbosity
 */
void print(double data, uint8_t digit, const Verbosity& verbosity) {
    if (!printPrefix(verbosity)) return;
    if (math::isnan(data)) {
        print("nan", Verbosity::Mute);
        return;
    }
    if (math::isinf(data)) {
        print("inf", Verbosity::Mute);
        return;
    }
    if (data > 4294967040.0) {
        print("ovf", Verbosity::Mute);// constant determined empirically
        return;
    }
    if (data < -4294967040.0) {
        print("ovf", Verbosity::Mute);// constant determined empirically
        return;
    }
    // Handle negative numbers
    if (data < 0.0) {
        print("-", Verbosity::Mute);
        data = -data;
    }
    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digit; ++i)
        rounding /= 10.0;
    data += rounding;
    // Extract the integer part of the number and print it
    auto int_part    = static_cast<uint32_t>(data);
    double remainder = data - static_cast<double>(int_part);
    print(int_part, IntFormat::Decimal, Verbosity::Mute);
    // Print the decimal point, but only if there are digits beyond
    if (digit > 0) {
        print(".", Verbosity::Mute);
    }
    // Extract digits from the remainder one at a time
    while (digit-- > 0) {
        remainder *= 10.0;
        auto toPrint = static_cast<unsigned int>(remainder);
        print(toPrint, IntFormat::Decimal, Verbosity::Mute);
        remainder -= toPrint;
    }
}

void logger(const char* str) { print(str, Verbosity::Mute); }
void logger(const string& str) { print(str, Verbosity::Mute); }
void logger(uint8_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(int8_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(uint16_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(int16_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(uint32_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(int32_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(uint64_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(int64_t data, const IntFormat& format) { print(data, format, Verbosity::Mute); }
void logger(double data, uint8_t digit) { print(data, digit, Verbosity::Mute); }

void warning(const char* str) { print(str, Verbosity::Warning); }
void warning(const string& str) { print(str, Verbosity::Warning); }
void warning(uint8_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(int8_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(uint16_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(int16_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(uint32_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(int32_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(uint64_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(int64_t data, const IntFormat& format) { print(data, format, Verbosity::Warning); }
void warning(double data, uint8_t digit) { print(data, digit, Verbosity::Warning); }

void error(const char* str) { print(str, Verbosity::Error); }
void error(const string& str) { print(str, Verbosity::Error); }
void error(uint8_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(int8_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(uint16_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(int16_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(uint32_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(int32_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(uint64_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(int64_t data, const IntFormat& format) { print(data, format, Verbosity::Error); }
void error(double data, uint8_t digit) { print(data, digit, Verbosity::Error); }

void debug(const char* str) { print(str, Verbosity::Debug); }
void debug(const string& str) { print(str, Verbosity::Debug); }
void debug(uint8_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(int8_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(uint16_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(int16_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(uint32_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(int32_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(uint64_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(int64_t data, const IntFormat& format) { print(data, format, Verbosity::Debug); }
void debug(double data, uint8_t digit) { print(data, digit, Verbosity::Debug); }

}// namespace sbs::io