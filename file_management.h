

#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <stdio.h>
#include <stdint.h>

typedef enum
{
    FileMgmt_Read,
    FileMgmt_Write
}FileMgmt_enOpenType;

typedef struct
{
    char* name;
    uint32_t size;
}FileMgmt_stFilename;

typedef struct
{
    FileMgmt_stFilename** files;
    uint32_t size;
}FileMgmt_stFiles;

/* Fonction pour ouvrir un fichier et renvoyer le gestionnaire de fichiers en cas de succès*/
FILE* FileMgmt_open(const char* start_dir,
                    const char* filename,
                    uint32_t size_filename,
                    FileMgmt_enOpenType openType);
/* Fonction pour fermer le gestionnaire de fichiers */
void FileMgmt_close(FILE* fh);
/* Fonction pour écrire dans le gestionnaire de fichiers*/
void FileMgmt_write(FILE* file, char* string, uint32_t size_string);
/* Fonction pour obtenir la liste des fichiers dans le répertoire de démarrage*/
FileMgmt_stFiles* FileMgmt_getListOfFiles(const char* start_dir,
                                          uint32_t max_size,
                                          uint32_t max_number_files);


void FileMgmt_cleanFiles(FileMgmt_stFiles* files_list);

#endif //FILE_MANAGEMENT_H
