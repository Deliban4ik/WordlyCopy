#include "wordle_api.h"
#include <cstring>
#include <iostream>

ruchar WordleGame::ALPHABET[WordleGame::ALPHABET_SIZE] = {
    {u8"а"[0], u8"а"[1]}, {u8"б"[0], u8"б"[1]}, {u8"в"[0], u8"в"[1]}, {u8"г"[0], u8"г"[1]},
    {u8"д"[0], u8"д"[1]}, {u8"е"[0], u8"е"[1]}, {u8"ё"[0], u8"ё"[1]}, {u8"ж"[0], u8"ж"[1]},
    {u8"з"[0], u8"з"[1]}, {u8"и"[0], u8"и"[1]}, {u8"й"[0], u8"й"[1]}, {u8"к"[0], u8"к"[1]},
    {u8"л"[0], u8"л"[1]}, {u8"м"[0], u8"м"[1]}, {u8"н"[0], u8"н"[1]}, {u8"о"[0], u8"о"[1]},
    {u8"п"[0], u8"п"[1]}, {u8"р"[0], u8"р"[1]}, {u8"с"[0], u8"с"[1]}, {u8"т"[0], u8"т"[1]},
    {u8"у"[0], u8"у"[1]}, {u8"ф"[0], u8"ф"[1]}, {u8"х"[0], u8"х"[1]}, {u8"ц"[0], u8"ц"[1]},
    {u8"ч"[0], u8"ч"[1]}, {u8"ш"[0], u8"ш"[1]}, {u8"щ"[0], u8"щ"[1]}, {u8"ъ"[0], u8"ъ"[1]},
    {u8"ы"[0], u8"ы"[1]}, {u8"ь"[0], u8"ь"[1]}, {u8"э"[0], u8"э"[1]}, {u8"ю"[0], u8"ю"[1]},
    {u8"я"[0], u8"я"[1]}
};

int WordleGame::getWordSize(const ruchar* seq) {
    int i = 0;
    while (!seq[i].end)
        i++;
    return i + 1;
}

int WordleGame::getWordSize(const char* seq) {
    int i = 0;
    while (seq[i] != '\0')
        i++;
    return i;
}

bool WordleGame::isEqual(ruchar a, ruchar b) {
    return (unsigned char)a.character[0] == (unsigned char)b.character[0]
        && (unsigned char)a.character[1] == (unsigned char)b.character[1];
}

ruchar* WordleGame::char2ruchar(const char* utf8str) {
    int byteLen = getWordSize(utf8str);
    int charCount = byteLen / 2;

    ruchar* result = (ruchar*)malloc(charCount * sizeof(ruchar));

    for (int i = 0; i < charCount; i++) {
        result[i].character[0] = utf8str[i * 2];
        result[i].character[1] = utf8str[i * 2 + 1];
        result[i].end = (i == charCount - 1);
    }

    return result;
}

ruchar* WordleGame::encodeWord(const ruchar* word, int length) {
    ruchar* encoded = (ruchar*)malloc(length * sizeof(ruchar));

    for (int i = 0; i < length; i++) {
        encoded[i].end = (i == length - 1);
        encoded[i].character[0] = ' ';
        encoded[i].character[1] = ' ';

        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (isEqual(word[i], ALPHABET[j])) {
                int shifted = (j + length) % ALPHABET_SIZE;
                encoded[i].character[0] = ALPHABET[shifted].character[0];
                encoded[i].character[1] = ALPHABET[shifted].character[1];
                break;
            }
        }
    }

    return encoded;
}

ruchar* WordleGame::decodeWord(const ruchar* encoded, int length) {
    ruchar* decoded = (ruchar*)malloc(length * sizeof(ruchar));

    for (int i = 0; i < length; i++) {
        decoded[i].end = (i == length - 1);
        decoded[i].character[0] = ' ';
        decoded[i].character[1] = ' ';

        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (isEqual(encoded[i], ALPHABET[j])) {
                int shifted = ((j - length) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;
                decoded[i].character[0] = ALPHABET[shifted].character[0];
                decoded[i].character[1] = ALPHABET[shifted].character[1];
                break;
            }
        }
    }

    return decoded;
}

WordleGame::WordleGame(const char* encodedWord) {
    ruchar* asRuchar = char2ruchar(encodedWord);
    int len = getWordSize(asRuchar);
    this->word_length = len;
    this->secret = decodeWord(asRuchar, len);
    free(asRuchar);
}

WordleGame::~WordleGame() {
    free(secret);
}

short* WordleGame::getCorrect(const ruchar* guess) const {
    short* output = (short*)malloc(word_length * sizeof(short));
    bool* used = (bool*)calloc(word_length, sizeof(bool));

    for (int i = 0; i < word_length; i++) {
        if (isEqual(guess[i], secret[i])) {
            output[i] = 2;
            used[i] = true;
        } else {
            output[i] = 0;
        }
    }

    for (int i = 0; i < word_length; i++) {
        if (output[i] == 2) continue;

        for (int j = 0; j < word_length; j++) {
            if (!used[j] && isEqual(guess[i], secret[j])) {
                output[i] = 1;
                used[j] = true;
                break;
            }
        }
    }

    free(used);
    return output;
}

bool WordleGame::isWin(const short* result) const {
    for (int i = 0; i < word_length; i++)
        if (result[i] != 2) return false;
    return true;
}
