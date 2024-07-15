#ifndef HASH_H
#define HASH_H

#include "helpers.h"
#include "aes.h"

//Davies-Meyers hash using AES
class Hash {
public:
    std::string hash(const std::string& input);

private:
    std::string pad(const std::string& input, const unsigned int bit_size);
    static std::string IV;
};


#endif
