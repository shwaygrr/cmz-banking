#ifndef DIGITALSIGNATURE_H
#define DIGITALSIGNATURE_H

#include <string>
#include "helpers.h"
#include "crypto/rsa.h"
#include "crypto/hash.h"
#include <QDebug>

class DigSig {
public:
    DigSig();

    void setRSA(const RSA& rsa_);
    void printRSA();
    std::string sign(const std::string& message);
    std::string sign(const std::string& message, RSA rsa_);
    bool verify(const std::string &message, const bigint& signature);
private:
    RSA rsa;
    Hash* hash;
};

#endif // DIGITALSIGNATURE_H
