#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "logica.h"

// Escolhe uma palavra aleatória de um arquivo
char *escolher_palavra_aleatoria(int dificuldade){
    
    // Abre o arquivo
    char caminho[100];
    sprintf(caminho, "lista_palavras/dificuldade_%d.txt", dificuldade);

    FILE *arquivo = fopen(caminho, "r");

    // Verifica por erros
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return NULL;
    }

    // Conta quantas palavras há no arquivo
    int contador = 0;
    char buffer[1000];
    
    while(fgets(buffer, sizeof(buffer), arquivo) != NULL){
        contador++;
    }

    fclose(arquivo);

    if (contador == 0) {
        printf("Arquivo vazio!\n");
        return NULL;
    }

    // Abre o arquivo novamente
    arquivo = fopen(caminho, "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo_2!\n");
        return NULL;
    }

    int qual_palavra = rand() % contador;
    for (int i = 0; i <= qual_palavra; i++) {
        if (fgets(buffer, sizeof(buffer), arquivo) == NULL) {
            fclose(arquivo);
            return NULL; // erro na leitura
        }
    }
    fclose(arquivo);

    // Aloca e copia a palavra escolhida
    buffer[strcspn(buffer, "\n")] = '\0'; // remove '\n'
    char *palavra_aleatoria = malloc(strlen(buffer) + 1);
    if (palavra_aleatoria == NULL) {
        printf("Erro ao alocar memória\n");
        return NULL;
    }
    strcpy(palavra_aleatoria, buffer);

    return palavra_aleatoria;
}

// Adiciona um caractere ao final de uma string
void add_string(char *string, char c){

    int tamanho = strlen(string);

    string[tamanho] = c;
    string[tamanho + 1] = '\0';

}

// Verifica se um caractere está na string
bool letra_esta_string(char *string, char letra){

    int tamanho = strlen(string);

    for(int i = 0; i < tamanho; i++){

        if(string[i] == letra){
            return true;
        }

    }

    return false;
}

// Criar a string que deve aparecer no jogo
char *palavra_impressao(char *string, char *achadas, bool *tudo_achado){

    *tudo_achado = true;

    if(string == NULL){
        printf("Palavra Nula\n");
        return NULL;
    }

    int len = strlen(string);

    char *impressao = malloc(sizeof(char) * (len + 1));

    if(impressao == NULL){
        printf("Erro ao alocar palavra_impressao\n");
        return NULL;
    }

    impressao[0] = '\0';

    // Verifica se cada caractere foi ou não achado antes
    for(int i = 0; i < len; i++){

        if(letra_esta_string(achadas, string[i])){
            add_string(impressao, string[i]);
        }else{
            *tudo_achado = false;
            add_string(impressao, '_');
        }

    }

    return impressao;
}