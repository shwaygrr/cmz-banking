#ifndef RSA_H
#define RSA_H
#include <iostream>
#include <string>
#include <random>
#include <QDebug>
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


    bigint encrypt(const bigint& plain_text, const bigint& e_pub_key_, const bigint& n_pub_key_);
    bigint decrypt(const bigint& cipher_text, const bigint& priv_key, const bigint& n_pub_key);

    bool verifyPrivateKey(const bigint& d_private_, const bigint& e_public_, const bigint& n_public_);
    //keys
    bigint n_public;
    bigint e_public;


    //setters
    void setPrivateKey(const bigint& d_private_);
    void setPublicKeys(const bigint& e_public_, const bigint& n_public_);

    bigint getPrivateKey() const;

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

    bigint d_private;
};

#endif // RSA_H
