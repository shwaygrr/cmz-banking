#ifndef AES_H
#define AES_H

#include <iostream>
#include <set>
#include <random>
#include <sstream>
#include <iomanip>

#include "bigint.h"
#include "helpers.h"

class AES128 {
public:
    //Mode of operation
    std::string encECB128(std::string message_hex);
    std::string encECB128(std::string message_hex, std::string priv_key_hex_);
    std::string decECB128(std::string message_hex);

    //AES-128 encryption
    std::bitset<128> aesEnc128(const std::string& plain_text128, const std::string& priv_key128);

    std::bitset<128> aesDec128(const std::string& cipher_text, const std::string& priv_key);

    void setPrivateKey(const std::string& priv_key_hex_);

    std::string generatePrivateKey();
private:
    std::string priv_key_hex;
    //tables
    static const std::vector<std::vector<std::string>> SBOX;
    static const std::vector<std::string> ROUND_CONSTANTS;
    static const std::vector<std::vector<std::string>> MIX;
    static const std::vector<std::vector<std::string>> INV_SBOX;
    static const std::vector<std::vector<std::string>> INV_MIX;

    //helper funtions
    std::bitset<8> gf8Mult(const std::string& mix_hex, const std::string& state_hex);
    std::string flipMatrix(const std::string& hex_str);

    //key generation
    std::bitset<32> g32(std::bitset<32> word_32, unsigned int round);
    std::string roundKey128(const std::string& prev_key_hex, unsigned int round);
    std::vector<std::string> keyGen(const std::string& key_hex);


    //encryption
    //round functions
    std::bitset<128> addRoundKey(const std::string& plain_text_hex, const std::string& key0_hex); //used in decryption

    template<size_t bit_size> //used in decryption
    void byteSub(std::bitset<bit_size>& bin, const std::vector<std::vector<std::string>>& table) {
        //get as hex
        std::string hex_rep = binToHex(bin);

        std::string hex_result = "";

        for(int i = 0; i < hex_rep.size(); i+=2) {
            std::string prev_byte = hex_rep.substr(i, 2);

            //get row and column
            unsigned int row, col;
            std::stringstream row_ss, col_ss;
            row_ss << std::hex << prev_byte[0]; col_ss << std::hex << prev_byte[1];
            row_ss >> row; col_ss >> col;

            hex_result += table[row][col];
        }
        // std::cout << hexToBin<bit_size>(hex_result);

        //turn to binary and return
        bin = hexToBin<bit_size>(hex_result);
    }

    void shiftRows(std::bitset<128>& bin128);

    void mixColumn(std::bitset<128>& bin128, const std::vector<std::vector<std::string>>& table); //used in decryption

    //decryption
    //inverse round functions
    void invShiftRows(std::bitset<128>& bin128) ;

    //AES-128 decryption

};
#endif // AES_H
