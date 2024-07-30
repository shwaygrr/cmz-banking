#ifndef RSA_H
#define RSA_H
#include <iostream>
#include <string>
#include <random>

#include "bigint.h"
#include "helpers.h"

class RSA {
public:
    RSA();

    //key gen
    void keyGen(unsigned int bit_size);

    //encryption
    bigint encrypt(const bigint& plain_text);

    //decryption
    bigint decrypt(const bigint& cipher_text);

    //keys
    bigint n_public;
    bigint e_public;
    bigint d_private;

    void test();
private:
    bigint modExp(const bigint& base, bigint exp, const bigint& modulus);
    bigint binaryToDecimal(const std::string& binary_str);
    bigint randOddNumGen(unsigned int size);
    bigint randNumGen(const int min, const bigint& max);
    bool millerRabin(const bigint& odd_num, unsigned int t);
    bigint gcd(const bigint &a, const bigint& b);
    bigint EEA(bigint a, bigint b);
    bigint randPrimeGen(unsigned int size);

};

#endif // RSA_H
