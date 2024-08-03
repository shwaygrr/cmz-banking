#include "crypto/hash.h"

//IV H_0
std::string Hash::IV = "A65BF65D2B18A5B48FCDFF8DF54796D6";

/*
    PKCS Padding
        - Input: Text and the bit size that the output should be a multiple of
        - Output: Hex padded with a bit size that is a multiple of the the bit size
*/
std::string Hash::pad(const std::string& input, const unsigned int bit_size) {
    std::string input_hex = textToHex(input);
    unsigned int hex_size =  bit_size/4;
    unsigned int pad_amt = 0;


    while ((input_hex.size() + pad_amt) % hex_size) pad_amt += 2;
    pad_amt/=2;

    if (!pad_amt) {
        for(int i = 0; i < hex_size; i+=2) input_hex += "10";
    } else {
        std::string pad_amt_hex = binToHex<8>(intToBin<8>(pad_amt));
        for (int i = 0; i < pad_amt; i++) input_hex += pad_amt_hex;
    }
    return input_hex;
}


/*
    Davies-Meyer Hash
        - Input: Text that need to hashed
        - Output: Hashed text
*/
std::string Hash::hash(const std::string& input, const std::string& salt) {
    std::string input_hex_padded = pad(input + salt, 128);

    AES128 aes;
    std::string H = IV;
    for(int i = 0; i < input_hex_padded.size(); i+=32) {
        std::string input_hex_block = input_hex_padded.substr(i, 32);
        H = binToHex<128>(aes.aesEnc128(input_hex_block, H) ^ hexToBin<128>(input_hex_block));
        qDebug() << H;
    }

    return H;
}
