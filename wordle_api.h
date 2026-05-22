#pragma once
#include <cstdlib>

struct ruchar {
    char character[2];
    bool end = false;
};

class WordleGame {
private:
    ruchar* secret;
    int word_length;

public:
    enum { ALPHABET_SIZE = 33 };
    static ruchar ALPHABET[ALPHABET_SIZE];

    static int getWordSize(const ruchar* seq);
    static int getWordSize(const char* seq);
    static bool isEqual(ruchar a, ruchar b);
    static ruchar* char2ruchar(const char* utf8str);
    static ruchar* encodeWord(const ruchar* word, int length);
    static ruchar* decodeWord(const ruchar* encoded, int length);

    explicit WordleGame(const char* encodedWord);

    ~WordleGame();

    int getWordLength() const { return word_length; }

    short* getCorrect(const ruchar* guess) const;

    bool isWin(const short* result) const;
};
