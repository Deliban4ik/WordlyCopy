#include <iostream>
#include <cstring>
// #include <cstdlib>
#include "wordle_api.h"
#include <windows.h>


static void printWord(const ruchar* word, int length) {
    for (int i = 0; i < length; i++)
        std::cout << word[i].character[0] << word[i].character[1];
}

static void printResult(const ruchar* guess, const short* colors, int length) {
    for (int i = 0; i < length; i++) {
        if (colors[i] == 2)      std::cout << "[=]";
        else if (colors[i] == 1) std::cout << "[~]";
        else                     std::cout << "[_]";
    }
    std::cout << "  ";
    printWord(guess, length);
    std::cout << "\n";
}

static ruchar* readGuess(int expectedLength) {
    char buf[128] = {};

    if (!(std::cin >> buf)) {
        std::cout << "\n[Конец ввода]\n";
        exit(0);
    }

    int byteLen = WordleGame::getWordSize(buf);
    int charLen = byteLen / 2;

    if (charLen != expectedLength) {
        std::cout << "Слово должно быть " << expectedLength << " букв! Попробуй ещё.\n";
        return nullptr;
    }

    return WordleGame::char2ruchar(buf);
}

int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    const int MAX_ATTEMPTS = 6;
    char encodedInput[128] = {};

    if (argc >= 2) {
        strncpy(encodedInput, argv[1], 127);
    } else {
        std::cout << "Введи зашифрованное слово: ";
        std::cin >> encodedInput;
    }

    WordleGame game(encodedInput);
    int wordLen = game.getWordLength();

    std::cout << "=== Угадайка ===\n";
    std::cout << "Угадай слово из " << wordLen << " букв. Попыток: " << MAX_ATTEMPTS << "\n";
    std::cout << "Подсказки: [=] на месте, [~] есть в слове, [_] нет в слове\n\n";

    bool won = false;
    int attempt = 0;

    while (attempt < MAX_ATTEMPTS && !won) {
        std::cout << "Попытка " << (attempt + 1) << "/" << MAX_ATTEMPTS << ": ";

        ruchar* guess = nullptr;
        while (guess == nullptr)
            guess = readGuess(wordLen);

        short* result = game.getCorrect(guess);
        printResult(guess, result, wordLen);

        if (game.isWin(result))
            won = true;

        free(result);
        free(guess);
        attempt++;
    }

    if (won) {
        std::cout << "\nСлово угадано за " << attempt << " попыток.\n";
    } else {
        std::cout << "\nИгра окончена. Слово было: ";
        ruchar* raw = WordleGame::char2ruchar(encodedInput);
        int len = WordleGame::getWordSize(raw);
        ruchar* answer = WordleGame::decodeWord(raw, len);
        printWord(answer, len);
        std::cout << "\n";
        free(answer);
        free(raw);
    }

    return 0;
}
