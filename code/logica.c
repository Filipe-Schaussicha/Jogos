#include "raylib.h"
#include "estruturas.h"
#include "logica.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Peca **iniciar_tabuleiro(){

    // aloca a matriz de peças
    Peca **tabuleiro = malloc(sizeof(Peca*) * 8);

    if(tabuleiro == NULL){
        return NULL;
    }

    for(int i = 0; i < 8; i++){

        tabuleiro[i] = malloc(sizeof(Peca) * 8);

        if(tabuleiro[i] == NULL){
            return NULL;
        }
    }

    // serve de referencia para preencher a primeira e última linha de peças
    TipoPeca disposicao_pecas[]= {
        TORRE, CAVALO, BISPO, DAMA, REI, BISPO, CAVALO, TORRE
    };

    // popula o array
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            bool cor;

            if(i < 4){
                cor = PRETO;
            }else{
                cor = BRANCO;
            }

            // popula a primeira e útima linha com as pesças do array anterior
            if(i == 0 || i == 7){
                tabuleiro[i][j] = (Peca){disposicao_pecas[j], cor};

            // popula a segunda e penúltima linha com peões 
            }else if(i == 1 || i == 6){
                tabuleiro[i][j] = (Peca){PEAO, cor};

            // popula o resto das linhas com a peça "NULA"
            }else{
                tabuleiro[i][j] = (Peca){NULA, NULA};
            }

            // para debug
            printf("Peca[%d][%d]: Tipo %d\n", i, j, tabuleiro[i][j].tipo);
        }
    }

    return tabuleiro;
}

Texture2D **carregar_imagens(){

    // aloca o array
    Texture2D **imagens = malloc(sizeof(Texture2D *) * 2);

    if(imagens == NULL) return NULL;

    for(int i = 0; i < 2; i++){
        imagens[i] = malloc(sizeof(Texture2D) * 6);
        if(imagens[i] == NULL) return NULL;
    }

    // lista dos nomes e cores (ver pasta pieces)
    char *nomes[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
    char *cores[] = {"white", "black"};
    char caminho[100];
    char cor;

    // Popula o array de Texture2D com as imagens carregadas
    for(int i = 0; i < 2; i++){

        for(int j = 0; j < 6; j++){

            sprintf(caminho, "pieces/%s-%s.png", cores[i], nomes[j]);

            imagens[i][j] = LoadTexture(caminho);
        }
    }

    return imagens;
}

bool compara_cores(Color cor1, Color cor2){

    // verifica se ambas cores são iguais
    if(cor1.r == cor2.r && cor1.g == cor2.g && cor1.b == cor2.b && cor1.a == cor2.a){
        return true;
    }

    return false;
}

bool **VerificarMovimentosPossiveis(Peca **tab, Vector2 peca){

    int x = (int)peca.x;
    int y = (int)peca.y;

    TipoPeca tipo = tab[y][x].tipo;

    bool cor_atual = tab[y][x].cor;

    bool **movimentos = malloc(sizeof(bool*) * 8);

    if(movimentos == NULL) return NULL;

    for(int i = 0; i < 8; i++){
        movimentos[i] = malloc(sizeof(bool) * 8);
        if(movimentos[i] == NULL) return NULL;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            movimentos[i][j] = 0;
        }
    }

    int sentido = 1;
    if(cor_atual == BRANCO){
        sentido *= -1;
    }

    if(tipo == PEAO){

        printf("peca.x: %d | peca.y: %d\n", (int)peca.x, (int)peca.y);

        if(tab[y + sentido][x].tipo == NULA){
            movimentos[y + sentido][x] = true;

            if(((y == 1 && sentido == 1) || (y == 6 && sentido == -1)) && tab[y + (2* sentido)][x].tipo == NULA){
                movimentos[y + (2* sentido)][x] = true;
            }
        }

        if(x != 7)
            if(tab[y + sentido][x + 1].tipo != NULA && tab[y + sentido][x + 1].cor == !cor_atual){
                movimentos[y + sentido][x + 1] = true;
            }

        if(x != 0)
            if(tab[y + sentido][x - 1].tipo != NULA && tab[y + sentido][x - 1].cor == !cor_atual){
                movimentos[y + sentido][x - 1] = true;
            }

        // TODO: outros movimentos do peão

        return movimentos;
    }

    // TODO: outras peças

    if(tipo == TORRE){

        for(int ty = y + 1; ty < 8; ty++){
            
            if(tab[ty][x].cor == cor_atual && tab[ty][x].tipo != NULA){
                break;
            }

            if(tab[ty][x].tipo != NULA && tab[ty - 1][x].tipo != NULA && ty - 1 != y){
                break;
            }

            movimentos[ty][x] = true;
        }

        for(int ty = y - 1; ty >= 0; ty--){
            
            if(tab[ty][x].cor == cor_atual && tab[ty][x].tipo != NULA){
                break;
            }

            if(tab[ty][x].tipo != NULA && tab[ty + 1][x].tipo != NULA && ty + 1 != y){
                break;
            }

            movimentos[ty][x] = true;
        }

        for(int tx = x + 1; tx < 8; tx++){
            
            if(tab[y][tx].cor == cor_atual && tab[y][tx].tipo != NULA){
                break;
            }

            if(tab[y][tx].tipo != NULA && tab[y - 1][tx].tipo != NULA && tx - 1 != x){
                break;
            }

            movimentos[y][tx] = true;
        }

        for(int tx = x - 1; tx >= 0; tx--){
            
            if(tab[y][tx].cor == cor_atual && tab[y][tx].tipo != NULA){
                break;
            }

            if(tab[y][tx].tipo != NULA && tab[y][tx + 1].tipo != NULA && tx + 1 != x){
                break;
            }

            movimentos[y][tx] = true;
        }

        return movimentos;
    }

    return NULL;
}

Peca **iniciar_array_pecas(){

    // aloca a matriz de peças
    Peca **array_pecas = malloc(sizeof(Peca*) * 2);

    if(array_pecas == NULL){
        return NULL;
    }

    for(int i = 0; i < 2; i++){

        array_pecas[i] = malloc(sizeof(Peca) * 8);

        if(array_pecas[i] == NULL){
            return NULL;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j =0; j < 8; j++){
            array_pecas[i][j] = (Peca){NULA, NULA};
        }
    }

    return array_pecas;
}