#include "crypto/aes.h"

//helper functions
/*
    Binary multiplacation using Galois Field
        Input: 2 Hex values one 4 bit and other 8 bits
        Ouput: Product GF(2^3)
*/
std::bitset<8> AES128::gf8Mult(const std::string& mix_hex, const std::string& state_hex) {
    std::bitset<8> mix8(hexToBin<8>(mix_hex)), state8(hexToBin<8>(state_hex));

    // std::cout << mix_hex << std::endl;
    if(mix_hex == "01") {
        return hexToBin<8>(state_hex);
    }

    //map to polynomials
    std::vector<int> mix_poly;//length should only be at most  2

    std::vector<int> state_poly;

    for(int i = 0; i < 8; i++) {
        if (mix8[i] == 1) mix_poly.push_back(i);
        if (state8[i] == 1) state_poly.push_back(i);
    }

    //distribution
    std::set<int> product_poly;

    const std::vector<int>irreducible_poly = {4, 3, 1, 0};

    //distrubute
    for(int i = 0; i < mix_poly.size(); i++) {
        for (int j = 0; j < state_poly.size(); j++) {
            if(product_poly.find(state_poly[j] + mix_poly[i]) != product_poly.end()) {
                product_poly.erase(product_poly.find(state_poly[j] + mix_poly[i])); //erase all instances if exist
            } else { //insert
                int sum = state_poly[j] + mix_poly[i];
                if (sum >= 8) {
                    std::vector<int> temp_poly;
                    int poly2 = sum-8;
                    if (poly2 >= 4) {
                        std::cout << "error var:poly2 too big" << poly2 << std::endl;
                        exit(0);
                    }
                    //distribute
                    for(int k = 0; k < 4; k++) {
                        temp_poly.push_back(irreducible_poly[k]+poly2);

                        if(product_poly.find(temp_poly[k]) != product_poly.end()) {
                            product_poly.erase(product_poly.find(temp_poly[k]));
                        } else {
                            product_poly.insert(temp_poly[k]);
                        }
                    }
                } else {
                    product_poly.insert(sum);
                }
            }
        }
    }

    // for(const int&n: product_poly) {
    //     std::cout << n << " ";
    // }

    //set bits
    std::bitset<8> answer;
    for(const int& bit: product_poly) answer[bit] = 1;

    // if (binToInt<8>(hexToBin<8>(mix_hex)) * binToInt<8>(hexToBin<8>(state_hex)) != binToInt<8>(answer)) {
    //     std::cout << binToInt<8>(hexToBin<8>(mix_hex)) << " * "
    //               << binToInt<8>(hexToBin<8>(state_hex)) << " != " << binToInt<8>(answer) << std::endl;
    // }

    return answer;
}


/*
    Create Matrix
        -Input: hex string
        -Output: basically flipped matrix
*/
std::string AES128::flipMatrix(const std::string& hex_str) {
    std::string hex_matrix;

    for(int i = 0; i < 8; i+=2) {
        for (int  j = i; j <= i+24; j+=8) {
            hex_matrix += hex_str.substr(j, 2);
            // std::cout << j << " ";
        }
        // std::cout <<std::endl;
    }
    return hex_matrix;
}



//key generation
/*
    g function for word3 in sub key generation (helper function for key generation)
        Input: 32-bit word
        Output: 32-bit word to xor with word1
*/
std::bitset<32> AES128::g32(std::bitset<32> word_32, unsigned int round) {
    //1 byte shift left
    word_32 = (word_32 << 8) | (word_32 >> (32 - 8));

    //byte substition
    byteSub<32>(word_32, SBOX);

    // std::cout << word_32;

    //xor with round constant
    word_32 = word_32 ^= hexToBin<32>(ROUND_CONSTANTS[round] + "000000");

    // std::cout << binToHex<32>(word_32) << std::endl;
    return word_32;
}


/*
    Subkey generation
        Input: Previous subkey, k_(i-1) as hex
        Ouput: Subkey, k_i as hex
*/
std::string AES128::roundKey128(const std::string& prev_key_hex, unsigned int round) {
    std::bitset<128> prev_key128(hexToBin<128>(prev_key_hex));

    //split into words
    std::string prev_key_str128 = prev_key128.to_string();
    std::bitset<32> word0_32(prev_key_str128.substr(0, 32)),
        word1_32(prev_key_str128.substr(32, 32)),
        word2_32(prev_key_str128.substr(64, 32)),
        word3_32(prev_key_str128.substr(96, 32));

    // std::cout << "word0: " << binToHex<32>(word0_32) << std::endl;
    // std::cout << "word1: " << binToHex<32>(word1_32) << std::endl;
    // std::cout << "word2: " << binToHex<32>(word2_32) << std::endl;
    // std::cout << "word3: " << binToHex<32>(word3_32) << std::endl;

    //get next words
    std::bitset<32> next_word0_32 = (word0_32 ^= g32(word3_32, round));
    std::bitset<32> next_word1_32 = (next_word0_32 ^ word1_32);
    std::bitset<32> next_word2_32 = (next_word1_32 ^ word2_32);
    std::bitset<32> next_word3_32 = (next_word2_32 ^ word3_32);

    std::bitset<128> key_i128(next_word0_32.to_string() + next_word1_32.to_string() + next_word2_32.to_string() + next_word3_32.to_string());

    return binToHex<128>(key_i128);
}


/*
    Key Generation
        Input: First key as hex
        Output: 10 round keys as hes
*/
std::vector<std::string> AES128::keyGen(const std::string& key_hex) {
    std::vector<std::string> keys = {key_hex};

    for(int i = 1; i <= 10; i++) {
        std::string key_i = roundKey128(keys[i-1], i-1);
        keys.push_back(key_i);
    }

    // for (const std::string& key: keys) {
    //     std::cout << key << std::endl;
    // }

    return keys;
}



// encryption
//round functions
/*
    Add Round Key
        -Input: 128 block of Plaintext and key0
        -Output: Plaintext xor key0
*/
std::bitset<128> AES128::addRoundKey(const std::string& plain_text_hex, const std::string& key_hex) {

    std::bitset<128> plain_text128(hexToBin<128>(plain_text_hex));
    std::bitset<128> key128(hexToBin<128>(key_hex));

    return plain_text128 ^ key128;
}


/*
    Shift rows
        Input: 128-bit binary
        Output: Four rows shifted cyclically to the left by offsets of 0,1,2, and 3
*/
void AES128::shiftRows(std::bitset<128>& bin128) {
    //split into words
    std::string str = bin128.to_string();
    std::bitset<32> word0_32(str.substr(0, 32)),
        word1_32(str.substr(32, 32)),
        word2_32(str.substr(64, 32)),
        word3_32(str.substr(96, 32));

    word1_32 = (word1_32 << 8) | (word1_32 >> (32 - 8));
    word2_32 = (word2_32 << (8 * 2)) | (word2_32 >> (32 - 8 * 2));
    word3_32 = (word3_32 << (8 * 3)) | (word3_32 >> (32 - 8 * 3));

    std::bitset<128> result(word0_32.to_string() + word1_32.to_string() + word2_32.to_string() + word3_32.to_string());

    bin128 = result;
    // std::cout << binToHex(result);

}


/*
    Mix Column
       -Input:
        -Output:
*/
void AES128::mixColumn(std::bitset<128>& bin128, const std::vector<std::vector<std::string>>& table) {
    // represent as matrix
    std::vector<std::vector<std::string>> hex_matrix;

    for(int i = 0; i < bin128.size(); i+=32) {
        std::bitset<32> byte_i(bin128.to_string().substr(i, 32));
        std::string word = binToHex(byte_i);

        std::vector<std::string> row;
        for(int j = 0; j < 8; j+=2){
            row.push_back(word.substr(j, 2));
        }
        hex_matrix.push_back(row);
    }


    //matrix mult
    std::string result_hex;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::bitset<8> col;
            for (int k = 0; k < 4; k++) {
                col ^= gf8Mult(table[i][k], hex_matrix[k][j]); //perform the multiplication and XOR operation
            }
            result_hex += (binToHex<8>(col));
        }
    }

    bin128 = hexToBin<128>(result_hex);
}

//AES-128 encryption
std::bitset<128> AES128::aesEnc128(const std::string& plain_text, const std::string& priv_key) {
    std::cout << "beginning encryption..." << std::endl;
    //generate keys
    std::vector<std::string> round_keys_hex = keyGen(priv_key);

    //create matrices
    std::string plain_text_matrix = flipMatrix(plain_text);
    std::string round_key_matrix = flipMatrix(round_keys_hex[0]);

    //add round with key0
    std::bitset<128> state_matrix128 = addRoundKey(plain_text_matrix, round_key_matrix);


    // 10 round enc
    for(int i = 1; i <= 10; i++) {
        byteSub(state_matrix128, SBOX);
        shiftRows(state_matrix128);
        //skip mixColumn last round
        if (i != 10) mixColumn(state_matrix128, MIX);

        state_matrix128 = addRoundKey(binToHex<128>(state_matrix128), flipMatrix(round_keys_hex[i]));
    }

    //flip matrix
    std::bitset<128> cipher_text128 = hexToBin<128>(flipMatrix(binToHex<128>(state_matrix128)));
    //return cipher text
    return cipher_text128;
}



//decryption
/*
    Inverse Shift rows
        Input: 128-bit binary
        Output: Four rows shifted cyclically to the right by offsets of 0,1,2, and 3
*/
void AES128::invShiftRows(std::bitset<128>& bin128) {
    //split into words
    std::string str = bin128.to_string();
    std::bitset<32> word0_32(str.substr(0, 32)),
        word1_32(str.substr(32, 32)),
        word2_32(str.substr(64, 32)),
        word3_32(str.substr(96, 32));

    //shift right each row
    word1_32 = (word1_32 >> 8) | (word1_32 << (32 - 8));
    word2_32 = (word2_32 >> (8 * 2)) | (word2_32 << (32 - 8 * 2));
    word3_32 = (word3_32 >> (8 * 3)) | (word3_32 << (32 - 8 * 3));

    std::bitset<128> result(word0_32.to_string() + word1_32.to_string() + word2_32.to_string() + word3_32.to_string());

    bin128 = result;
}


//AES-128 decryption
std::bitset<128> AES128::aesDec128(const std::string& cipher_text, const std::string& priv_key) {
    std::cout << "beginning decryption..." << std::endl;

    //generate keys
    std::vector<std::string> round_keys_hex = keyGen(priv_key);

    //create matrices **not necessary**
    std::string plain_text_matrix = flipMatrix(cipher_text);
    std::string round_key_matrix = flipMatrix(round_keys_hex[10]); //start with last key

    //add round with key10
    std::bitset<128> state_matrix128 = addRoundKey(plain_text_matrix, round_key_matrix); //good

    // 10 round enc
    for(int i = 9; i >= 0; i--) {
        // std::cout << "round " << 10-i << " key: " << round_keys_hex[i] << std::endl;

        invShiftRows(state_matrix128);
        // std::cout << "after shift rows " << 10-i << ": " << flipMatrix(binToHex<128>(state_matrix128)) << std::endl;

        byteSub(state_matrix128, INV_SBOX);
        // std::cout << "after byte sub " << 10-i << ": " << flipMatrix(binToHex<128>(state_matrix128)) << std::endl;

        // std::cout << "after add round key " << 10-i << ": " << flipMatrix(binToHex<128>(state_matrix128)) << std::endl << std::endl;
        state_matrix128 = addRoundKey(binToHex<128>(state_matrix128), flipMatrix(round_keys_hex[i]));

        //skip mixColumn last round
        if (i != 0) {
            mixColumn(state_matrix128, INV_MIX);
            // std::cout << "after mix col " << 10-i << ": " << flipMatrix(binToHex<128>(state_matrix128)) << std::endl;
        }
    }

    // std::cout << "----------------------------------------------------" << std::endl;

    //flip matrix back
    std::bitset<128> plain_text128 = hexToBin<128>(flipMatrix(binToHex<128>(state_matrix128)));

    return plain_text128;
}


/*
    Electronic Code Block Mode of Operation
        - Input: 128x-bit Plain text as hex and 128-bit hex
        - Output: 128x-bit Cipher text as hex
*/
std::string AES128::encECB128(std::string message_hex, std::string priv_key_hex) {

    //pad message and key
    while (message_hex.size() % 32 != 0) message_hex = "0" + message_hex;
    while (priv_key_hex.size() % 32 != 0) priv_key_hex = "0" + priv_key_hex;

    //start encryption 128 bits at a time
    std::string cipher_text = "";

    for(int i = 0; i < message_hex.length(); i+=32) {
        //get 32-hexadecimal block
        std::string cipher_hex_block = message_hex.substr(i, 32);

        //encrypt
        cipher_hex_block = binToHex<128>(aesEnc128(cipher_hex_block, priv_key_hex));

        //add block
        cipher_text += cipher_hex_block;
    }

    return cipher_text;
}


/*
    Electronic Code Block Mode of Operation
        - Input: 128x-bit Cipher text as hex and 128-bit hex
        - Output: 128x-bit Plain text as hex
*/
std::string AES128::decECB128(std::string message_hex, std::string priv_key_hex) {

    //pad message and key
    while (message_hex.size() % 32 != 0) message_hex = "0" + message_hex;
    while (priv_key_hex.size() % 32 != 0) priv_key_hex = "0" + priv_key_hex;

    // std::cout << message_hex

    //start encryption 128 bits at a time
    std::string plain_text = "";

    for(int i = 0; i < message_hex.length(); i+=32) {
        //get 32-hexadecimal block
        std::string plain_hex_block = message_hex.substr(i, 32);

        //encrypt
        plain_hex_block = binToHex<128>(aesDec128(plain_hex_block, priv_key_hex));

        //turn block to text
        plain_text += plain_hex_block;
    }

    return plain_text;
}
