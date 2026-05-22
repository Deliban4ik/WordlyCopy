#include <iostream>
#include <cstring>
#include "wordle_api.h"
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    char input[128] = {};
    if (argc >= 2) {
        strncpy(input, argv[1], 127);
    } else {
        std::cout << "Введи слово для шифрования: ";
        std::cin >> input;
    }

    ruchar* word = WordleGame::char2ruchar(input);
    int len = WordleGame::getWordSize(word);
    ruchar* encoded = WordleGame::encodeWord(word, len);

    std::cout << "Зашифровано: ";
    for (int i = 0; i < len; i++)
        std::cout << encoded[i].character[0] << encoded[i].character[1];
    std::cout << "\n";

    free(encoded);
    free(word);
    return 0;
}
