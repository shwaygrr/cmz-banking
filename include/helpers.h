#ifndef HELPERS_H
#define HELPERS_H

#include "crypto/bigint.h"
#include <iostream>
#include <bitset>
#include <regex>

/*******************************************CONVERSIONS************************************************/
/*
    Binary to Decimal
        Input: Binary
        Output: Decimal representation
*/
template<size_t bit_size>
bigint binToInt(const std::bitset<bit_size>& bin) {
    bigint result = 0;

    bigint base(2);
    for (int i = 0; i < bit_size; i++) {
        bigint power(i);
        if (bin[i]) {
            result += big_pow(base, power);
        }
    }
    return result;
}


/*
    Binary to Hexadecimal
        - Input: binary
        - Output: hexadecimal representation
*/
template<size_t bit_size>
std::string binToHex(const std::bitset<bit_size>& bin) {
    std::string bin_str = bin.to_string();

    //pad
    while (bin_str.size() % 4 != 0) bin_str = "0" + bin_str;

    std::string hex_str;

    //hex chunks of 4 bits
    for (int i = 0; i < bin_str.size(); i += 4) {
        //extract a chunk of 4 bits from the binary number
        std::bitset<4> chunk(bin_str.substr(i, 4));

        //chunk to decimal
        char hex_digit = chunk.to_ulong() < 10 ? '0' + chunk.to_ulong() : 'A' + chunk.to_ulong() - 10;

        hex_str.push_back(hex_digit);
    }

    // Return the hexadecimal string
    return hex_str;
}


/*
    Integer to Binary
        Input: Integer
        Output: Binary representation
*/
template<size_t bit_size>
std::bitset<bit_size> intToBin(bigint integer) {
    std::string result;

    while(integer != 0) {
        result = (integer % 2).as_str() + result;
        integer /= 2;
    }
    // std::cout << result;
    std::bitset<bit_size> result_bin(result);
    return result_bin;
}


/*
    Hexadecimal to Binary
        - Input: Hexadecimal as string
        - Output: Binary representation
*/
template<size_t bit_size>
std::bitset<bit_size> hexToBin(const std::string& hex) {
    std::string result_str = "";

    //convert to int
    for(const char& hex_letter : hex) {
        //convert to int
        int int_value;
        std::stringstream int_value_ss;
        int_value_ss << std::hex << hex_letter;
        int_value_ss  >> int_value;

        std::string int_value_bin = intToBin<4>(int_value).to_string();

        result_str += int_value_bin;
    }

    //int to binary
    std::bitset<bit_size> result(result_str);
    return result;
}


std::string hexToText(std::string hex);
std::string textToHex(std::string text);
/*****************************************CONVERSIONS END*******************************************************/

bool checkPasswordFormat(const std::string& password);
bool checkEmailFormat(const std::string& email);
std::string generateSalt(const int length = 16);
#endif // HELPERS_H
