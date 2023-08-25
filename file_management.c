
#ifdef __WIN32
#include <Windows.h>
#include <direct.h>
#endif
#include <stdio.h>
#include "file_management.h"

FILE* FileMgmt_open(const char* start_dir,
                    const char* filename,
                    uint32_t size_filename,
                    FileMgmt_enOpenType openType)
{
    FILE* result = NULL;
    char f[50] = { 0 };
    if (start_dir) {
        strcpy_s(f, 50, start_dir);
        strcat_s(f, 50, "/");
    }
    strcat_s(f, 50, filename);
    if (openType == FileMgmt_Write) {
        result = fopen(f, "w");
    } else if (openType == FileMgmt_Read) {
        result = fopen(f, "r");
    }
    return result;
}

void FileMgmt_close(FILE* fh)
{
    if (fh) {
        fclose(fh);
        fh = NULL;
    }
}


void FileMgmt_write(FILE* file, char* string, uint32_t size_string)
{
    if (file != NULL) {
        for (int i = 0; i < size_string; i++) {
            putc(string[i], file);
        }
    }
}

FileMgmt_stFiles* FileMgmt_getListOfFiles(const char* start_dir, uint32_t max_size, uint32_t max_number_files)
{
    FileMgmt_stFiles* result = NULL;
    char sPath[200];
#ifdef WIN32
    result = (FileMgmt_stFiles*)calloc(1, sizeof(FileMgmt_stFiles));
    if (result == NULL) return result;
    result->files = calloc(max_number_files, sizeof(FileMgmt_stFilename));
    uint32_t number_elements = 0;

    WIN32_FIND_DATA fData;
    HANDLE hFind = NULL;
    strncpy_s(sPath, 200, start_dir, max_size);
    strcat_s(sPath, 200, "\\battleship_*");
    hFind = FindFirstFile(sPath, &fData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Route non trouvé\n");
    } else {
        do {
            // If it does not have the directory flag... it is a file
            if(!(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                result->files[number_elements] = (FileMgmt_stFilename*)calloc(1, sizeof(FileMgmt_stFilename));
                result->files[number_elements]->name = (char*)calloc(strlen(fData.cFileName) + 1, sizeof(char));
                result->files[number_elements]->size = strlen(fData.cFileName);
                strcpy_s(result->files[number_elements]->name,
                         result->files[number_elements]->size + 1,
                         fData.cFileName);
                memset(sPath, 0, result->files[number_elements]->size);
                number_elements++;
            } else {
                printf("Directory: %s\n", sPath);
            }
        } while(FindNextFile(hFind, &fData) && number_elements < max_number_files);
        result->size = number_elements;
    }
    FindClose(hFind);
#endif
    return result;
}

void FileMgmt_cleanFiles(FileMgmt_stFiles* files_list)
{
    if (files_list) {
        for (uint32_t i = 0; i < files_list->size; i++) {
            free(files_list->files[i]->name);
            free(files_list->files[i]);
        }
        free(files_list->files);
        free(files_list);
        files_list = NULL;
    }
}


