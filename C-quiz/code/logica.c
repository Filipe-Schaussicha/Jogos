#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "logica.h"
#include "visual.h"
#include "raylib.h"
#include <stdlib.h>

int contar_dir(){

    const char *path = "./questoes/";
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    // --- Primeira passada: contar pastas ---
    dir = opendir(path);
    if (dir == NULL) {
        perror("Não foi possível abrir o diretório");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_type == DT_DIR) {
            count++;
        }
    }
    closedir(dir);

    return count;

}

char *ler_dir(int max){

    const char *path = "./questoes/";
    DIR *dir;
    struct dirent *entry;

    // --- Segunda passada: listar pastas ---
    dir = opendir(path);
    if (dir == NULL) {
        perror("Não foi possível abrir o diretório");
        return NULL;
    }

    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        if (entry->d_type == DT_DIR) {
            if(i == max){
                return entry->d_name;
            }

            i++;
        }
    }
    closedir(dir);

    return NULL;
}

Font carrega_fonte(char *caminho, int tamanho){

    return LoadFontEx(caminho, tamanho, NULL, 0);
}