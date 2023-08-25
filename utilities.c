#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "utilities.h"

#define MAX_NUMBER_OF_TOKENS (6)

bool Utils_isANumber(char *string, uint32_t max_size)
{
    bool result = TRUE;
    uint32_t index = 0;

    while (string[index] != '\0' && index < max_size)
    {
        if (string[index] < 0x30 || string[index] > 0x39)
        {
            result = FALSE;
            break;
        }
        index++;
    }

    return result;
}

uint32_t Utils_getUserInput(char *query, char *userInput, uint32_t max_size)
{
    uint32_t size = 0;
    printf("%s", query);
    int ch;

    while ((ch = getchar()) != EOF)
    {
        if (size < max_size - 1) {
            userInput[size] = (char)ch;
        }
        if (ch == '\n') {
            break;
        }
        size++;
    }
    userInput[size] = '\0';

    return size;
}

uint32_t Utils_removeSpaces(char* string, uint32_t max_size)
{
    uint32_t new_size = 0;

    if (string != NULL) {
        for (int i = 0; i < max_size; i++)
        {

            if (string[i] == 0) {
                break;
            }
            if (string[i] != ' ') {
                string[new_size++] = string[i];
            }
        }
    }
    return new_size;
}

uint32_t Utils_getLine(FILE* fh, char* string, uint32_t max_size)
{
    uint32_t size = 0;
    if (fh && string) {
        char c;
        do {
            c = (char)getc(fh);
            if (c != '\n' && c != EOF && size < max_size) {
                string[size++] = c;
            }
        } while (c != EOF && c != '\n');
    }
    return size;
}

Utils_stTokenizer* Utils_tokenize(const char* string, uint32_t max_size, char token)
{
    Utils_stTokenizer* tokenizer = NULL;
    uint32_t start = 0;
    uint32_t index = 0;
    uint32_t number_of_elements = 0;
    uint32_t size = 0;
    if (string) {
        tokenizer = (Utils_stTokenizer*)calloc(1, sizeof(Utils_stTokenizer));
        if (tokenizer) {
            tokenizer->tokens = (Utils_stToken**)calloc(MAX_NUMBER_OF_TOKENS, sizeof (Utils_stToken));
            char c;
            while (index < max_size && number_of_elements < MAX_NUMBER_OF_TOKENS) {
                c = string[index];
                if (c == token) {
                    size = index - start;
                    tokenizer->tokens[number_of_elements] = (Utils_stToken*) calloc(1, sizeof (Utils_stToken));
                    tokenizer->tokens[number_of_elements]->data = (char*) calloc(size + 1,
                                                                                 sizeof (char));
                    memcpy(tokenizer->tokens[number_of_elements]->data,
                           string + start,
                           size);
                    tokenizer->tokens[number_of_elements]->data[size] = '\0';
                    tokenizer->tokens[number_of_elements]->size = size;
                    number_of_elements++;
                    start = index + 1;
                }
                index++;
            }
            if (start < index) {
                size = index - start;
                tokenizer->tokens[number_of_elements] = (Utils_stToken*) calloc(1, sizeof (Utils_stToken));
                tokenizer->tokens[number_of_elements]->data = (char*) calloc(size + 1, sizeof (char));
                memcpy(tokenizer->tokens[number_of_elements]->data,
                       string + start,
                       size);
                tokenizer->tokens[number_of_elements]->data[size] = '\0';
                tokenizer->tokens[number_of_elements]->size = size;
                number_of_elements++;
            }
        }
        if (number_of_elements == 0 && tokenizer != NULL) {
            free(tokenizer);
            tokenizer = NULL;
        } else {
            tokenizer->size = number_of_elements;
        }
    }


    return tokenizer;
}

void Utils_cleanTokenizer(Utils_stTokenizer* tokenizer)
{
    if (tokenizer) {
        for (uint32_t i = 0; i < tokenizer->size; i++) {
            free(tokenizer->tokens[i]->data);
            free(tokenizer->tokens[i]);
        }
        free(tokenizer->tokens);
        free(tokenizer);
        tokenizer = NULL;
    }
}
