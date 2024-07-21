#include "helpers.h"

/*
    Text to Hexadecimal
        -Input: Text string
        -Output: haxadeicmal rep value of char's ascii s
*/
std::string textToHex(std::string text) {
    std::string result = "";;

    for (int i = 0; i < text.size(); i++) {
        //hex to int
        int ascii = static_cast<int>(static_cast<unsigned char>(text[i]));

        //int to hex
        std::string ascii_hex = binToHex<8>(intToBin<8>(ascii));
        // std::cout << ascii_hex << std::endl;
        // std::cout << static_cast<char>(ascii) << std::endl;
        result += ascii_hex;
    }
    return result;
}


/*
    Hexadcimal to Text
        -Input: hexadecimal string
        -Output: ascii rep of each b-bit hexadecimal

*/
std::string hexToText(std::string hex) {
    //pad hex
    while (hex.size() % 2 != 0) hex = "0" + hex;

    std::string result = "";
    for (int i = 0; i < hex.size(); i+=2) {
        std::string hex_i = hex.substr(i, 2);

        //turn to int
        std::bitset<8> hex_i_bin(hexToBin<8>(hex_i));
        bigint hex_i_int = binToInt<8>(hex_i_bin); //might have to add 32
        // std::cout << hex_int << std::endl;
        //int to ascii
        result += static_cast<char>(stoi(hex_i_int.as_str()));
    }

    //turn every two to decimal and get ascii rep
    return result;
}


//Check Password Format
bool checkPasswordFormat(const std::string& password) {
    // Check if the password is at least 8 characters long
    if (password.size() < 8) return false;

    // - at least one digit
    // - at least one uppercase letter
    // - at least one special character
    std::regex passwordRegex("^(?=.*[0-9])(?=.*[A-Z])(?=.*[^a-zA-Z0-9]).{8,}$");

    return std::regex_match(password, passwordRegex);
}

//Check email format -- rememeber to change return type
bool checkEmailFormat(const std::string &email) {
    const std::regex emailRegex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");

    return std::regex_match(email, emailRegex);
}
