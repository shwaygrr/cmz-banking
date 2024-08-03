#include "crypto/digsig.h"

DigSig::DigSig() {
    rsa = RSA();
    hash = new Hash();
}

std::string DigSig::sign(const std::string& message) {
    std::string message_hash = hash->hash(message);
    bigint message_hash_int = binToInt<128>(hexToBin<128>(message_hash));
    return rsa.sign(message_hash_int).as_str();
}

std::string DigSig::sign(const std::string& message, RSA rsa_) {
    std::string message_hash = hash->hash(message);
    bigint message_hash_int = binToInt<128>(hexToBin<128>(message_hash));

    return rsa_.sign(message_hash_int).as_str();
}

void DigSig::setRSA(const RSA& rsa_) { rsa = rsa_; }

void DigSig::printRSA() {
    qDebug() << "print rsa";
    qDebug() << QString::fromStdString(rsa.getPrivateKey().as_str());
    qDebug() << QString::fromStdString(rsa.e_public.as_str());
    qDebug() << QString::fromStdString(rsa.n_public.as_str());
}

bool DigSig::verify(const std::string &message, const bigint& signature) {
    printRSA();
    std::string message_hash = hash->hash(message);
    bigint signature_unsigned = rsa.unsign(signature).as_str();

    qDebug() << "message hash" << QString::fromStdString(message_hash);
    qDebug() << "unsigned sig" << QString::fromStdString(binToHex<128>(intToBin<128>(signature_unsigned)));

    return message_hash == binToHex<128>(intToBin<128>(signature_unsigned));
}
