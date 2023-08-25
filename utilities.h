#ifndef UTILITIES_H
#define UTILITIES_H

#include "battleship_types.h"

typedef struct
{
    char* data;
    uint32_t size;
}Utils_stToken;

typedef struct
{
    Utils_stToken** tokens;
    uint32_t size;
}Utils_stTokenizer;

/***************************
 * Function declarations
 **************************/


bool Utils_isANumber(char* string, uint32_t max_size);

uint32_t Utils_getUserInput(char* query, char* userInput, uint32_t max_size);


uint32_t Utils_removeSpaces(char* string, uint32_t max_size);


uint32_t Utils_getLine(FILE* fh, char* string, uint32_t max_size);


Utils_stTokenizer* Utils_tokenize(const char* string, uint32_t max_size, char token);

void Utils_cleanTokenizer(Utils_stTokenizer* tokenizer);

#endif //UTILITIES_H
