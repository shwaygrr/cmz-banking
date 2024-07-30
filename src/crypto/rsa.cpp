#include "crypto/rsa.h"

/*
    Modular Exponentiation
        -Input: Base, Exponent, and Modulus
        -Output Base^Exponent mod Modulus
*/
RSA::RSA() {
    keyGen(30); //low bit size for speed
}

void RSA::test() {
    // randPrimeGen(4);
    bool isPrime = millerRabin(10, 50);


    std::cout << "miller-rabin: " << isPrime;
}

bigint RSA::modExp(const bigint& base, bigint exp, const bigint& modulus) {
    bigint result = 1;

    if (exp == 0) return result;

    bigint A = base;

    if (exp % 2 == 1) {
        result = base;
    }

    exp /= 2;
    std::cout << "exp: "  << exp << std::endl;
    while (exp != 0) {
        A = (A*A) % modulus;

        if (exp % 2 == 1) result = (A*result) % modulus;

        exp /= 2; //next bit
        std::cout << "Result = " << result << std::endl;

    }
    return result;
}


/*
    Binary to Decimal
        - Input: binary string
        - Output: large bigint
        https://www.geeksforgeeks.org/program-binary-decimal-conversion/
*/
bigint RSA::binaryToDecimal(const std::string& binary_str) {
    bigint dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    bigint base = 1;

    int len = binary_str.length();
    for (int i = len - 1; i >= 0; i--) {
        if (binary_str[i] == '1')
            dec_value += base;
        base = base * 2;
    }

    return dec_value;
}


/*
    Random Odd Number Generator
        - input: bit size, size
        - output: random odd number of bit size, size
        https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
*/
bigint RSA::randOddNumGen(unsigned int size) {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    std::uniform_int_distribution<unsigned short int> distrib(0,1);

    // return distrib(gen);
    std::string binary_str = "1";

    // Iterate over the range [0, N - 1]
    for (int i = 0; i < size-2; i++) {
        binary_str += std::to_string(distrib(gen));
    }

    binary_str += "1"; //ensure odd (prime)

    // std::cout << binaryToDecimal(binary_str).as_str().length() << std::endl;
    return binaryToDecimal(binary_str); //converted to int
}


/*
    Random Number Generator
        - input: min and max
        - output: random number between min and max inclusive
        https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        **implementation can be improved**
*/
bigint RSA::randNumGen(const int min, const bigint& max) {
    //error handling
    if (min >= max) {
        throw std::invalid_argument("min must be less than max");
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    //number of bits required to represent max
    std::string max_bit_size = (big_log2(max)+1).as_str();

    //generate a random number within range
    bigint rand_num;

    do {
        std::string binary_str;
        for (int i = 0; i < std::stoi(max_bit_size); i++) {
            binary_str += std::to_string(gen() % 2);
        }
        rand_num = binaryToDecimal(binary_str);
    } while (rand_num < min || rand_num > max);

    return rand_num;
}


/*
    Miller-Rabin primality test
        -Input: ineger (odd number in this case), amout a tests, t
        -Output: boolean for is prime
*/
bool RSA::millerRabin(const bigint& odd_num, unsigned int t) {
    // std::cout << "miller rabin" << std::endl;
    if (odd_num <= 1)
        return false;
    if (odd_num <= 3)
        return true;

    //determine the values of s and r such that odd_num - 1 = 2^s * r
    bigint s = 0, r = odd_num - 1;
    while (r % 2 == 0) {
        r /= 2;
        ++s;
    }

    for (unsigned int i = 0; i < t; ++i) {
        bigint rand_num = randNumGen(2, odd_num - 1); //random number in the range [2, odd_num - 1]
        bigint y = modExp(rand_num, r, odd_num);

        if (y != 1 && y != odd_num - 1) {
            bigint j = 1;
            while (j <= s - 1 && y != odd_num - 1) {
                y = modExp(y, 2, odd_num);
                if (y == 1)
                    return false;
                ++j;
            }
            if (y != odd_num - 1){
                return false;}
        }
    }
    return true;
}


/*
    Stein’s algorithm or binary GCD algorithm is an algorithm that computes the greatest common divisor of two non-negative integers.
    Stein’s algorithm replaces division with arithmetic shifts, comparisons, and subtraction.
    https://www.geeksforgeeks.org/steins-algorithm-for-finding-gcd/
*/
bigint RSA::gcd(const bigint &a, const bigint& b) {
    std::cout << "(" << a << ", " << b << ")" << std::endl;
    std::bitset<512> a_bin = intToBin<512>(a), b_bin = intToBin<512>(b);

    /*Finding K, where K is the
    greatest power of 2
    that divides both a and b. */
    int k;
    for (k = 0; ((a_bin | b_bin) & std::bitset<512>(1)) == 0; ++k) {
        a_bin >>= 1;
        b_bin >>= 1;
    }

    /* Dividing a by 2 until a becomes odd */
    while ((a_bin & std::bitset<512>(1)) == 0) a_bin >>= 1;

    /* From here on, 'a' is always odd. */
    do {
        std::cout << binToInt<512>(b_bin) << std::endl;
        /* If b is even, remove all factor of 2 in b */
        while ((b_bin & std::bitset<512>(1)) == 0) b_bin >>= 1;
        /* Now a and b are both odd.
        Swap if necessary so a <= b,
        then set b = b - a (which is even).*/
        if (binToInt<512>(a_bin) > binToInt<512>(b_bin)) {
            std::bitset<512> temp = a_bin;
            a_bin = b_bin;
            b_bin = temp;
        }


        b_bin = intToBin<512>(binToInt<512>(b_bin) - binToInt<512>(a_bin));
    } while (b_bin != 0);

    /* restore common factors of 2 */
    std::cout << binToInt<512>(a_bin << k) << std::endl;
    return binToInt<512>(a_bin << k);
}


/*
    Extended Euclidean
        -Input: two non-negative integer a and b : a >= b
        -Output: d = gcd(a, b) and integers x, y satisfying ax + by = d
        **Algorithm from Handbook of Applied Cryptography**
*/
bigint RSA::EEA(bigint a, bigint b) {
    std::cout << "calculating inverse..." << std::endl;
    bigint d, x, y;


    if (b == 0) {
        d = a; x = 1; y = 0;
        return x;
    }

    bigint x2 = 1, x1 = 0, y2 = 0, y1 = 1;
    while (b > 0) {
        bigint q = a/b;
        bigint r = a - (q*b);
        x = x2 - (q*x1);
        y = y2 - (q*y1);

        a = b; b = r; x2 = x1; x1 = x; y2 = y1; y1 = y;
    }

    d = a; x = x2; y = y2;

    if (x > 0) return x; else return y;
}


//key generation
/*
    Large Prime Generator
        -Input: bit size, n
        -Output: prime number of bitsize n tested by Miller-Rabin
*/
bigint RSA::randPrimeGen(unsigned int size) {
    bigint rand_num;

    do {
        rand_num = randOddNumGen(size);
        std::cout << "testing " << rand_num << std::endl;
    } while(rand_num % 3 == 0 || rand_num % 5 == 0 || millerRabin(rand_num, 50) == false ); //maybe disqualigy multiples of 5

    std::cout << "Found Prime: " << rand_num << std::endl;

    return rand_num;
}


//encryption
/*
    RSA Key Generation
        -Input: key size in bits
        -Output: Set public and private keys
*/
void RSA::keyGen(unsigned int bit_size) {

    //generate two large primes
    bigint p = randPrimeGen(bit_size);
    bigint q = randPrimeGen(bit_size);

    //tested primes
    // bigint p("70343393710718721739938719193169510179855281731980269448074487233019254740753");
    // bigint q("58221670538539099290977277207231427756011693475693578834912044130655753867843");

    //ensure p and q are distinct
    while (p == q) q = randPrimeGen(bit_size);

    //compute n and phi = (p-1)*(q-1)
    n_public = p * q;
    bigint phi = (p - 1) * (q - 1);

    //generate public and private keys
    do {
        //generate random e
        do {
            std::cout << "getting e" << std::endl;
            e_public = randNumGen(2, phi - 1);
        } while (gcd(phi, e_public) != 1);

        //compute inverse
        d_private = EEA(phi, e_public);
        std::cout << "e_public * d_private) % phi = " << (e_public * d_private) % phi << std::endl;
    } while ((e_public * d_private) % phi != 1); //check if e*d mod phi = 1

    // e_public = bigint("4095509893188208770005594148806912901940254590834413582211574248986413173696675736731560807225744215275640986373996205331286759337101744549032093988305779");
    // d_private = bigint("579435979162619761190367120345111303949825465176877687963471681484228319408082774616630434770887313002355665524535712929045856226063476796034002666882897");

    std::cout << "n_public: " << n_public << std::endl;
    std::cout << "e_public: " << e_public << std::endl;
    std::cout << "d_private: " << d_private << std::endl;
    // std::cout << "Verification: " << (d_private * e_public) % phi << std::endl;
}


/*
    RSA Encryption
        Input: Message plain text as integer
        Output: Cipher text interger
    //obtain authentic public key **digital signiture**
*/
bigint RSA::encrypt(const bigint& plain_text) {
    std::cout << "rsa encryption..." << std::endl;
    return modExp(plain_text, e_public, n_public);
}


/*
    RSA Decryption
        Input: Cipher Text as integer
        Output: plain text ass integer (original message)
*/
bigint RSA::decrypt(const bigint& cipher_text) {
    std::cout << "rsa decrypting..." << std::endl;
    return modExp(cipher_text, d_private, n_public);
}
