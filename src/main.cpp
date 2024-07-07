// #include "mainwindow.h"
// #include <QApplication>

// using namespace std;

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     MainWindow w;

//     w.show();

//     return a.exec();
// }



#include "crypto/aes.h"
#include "helpers.h"
#include <QDebug>
// #include <QApplication>


int main() {

    AES128 a;

    std::string message = "Thats my Kunrg Fu";
    std::string key = "Two One Nine Two";

    std::string message_hex = textToHex(message);

    std::string key_hex = textToHex(key);

    std::string cipher = a.encECB128("00112233445566778899aabbccddeeff", "000102030405060708090a0b0c0d0e0f");
    std::string dec = a.decECB128(cipher, "000102030405060708090a0b0c0d0e0f");

    // std::cout << "message: " << message_hex << std::endl;
    // std::cout << "key: "  << key_hex << std::endl;
    std::cout << "00112233445566778899aabbccddeeff" << "---ENC--->" << cipher << std::endl << cipher << "---DEC--->" << dec;
    return 0;
}
