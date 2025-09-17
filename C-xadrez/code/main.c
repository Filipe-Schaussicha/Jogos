#include "raylib.h"
#include "estruturas.h"
#include "visual.h"
#include "logica.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

    InitWindow(SW, SH, "C-Xadrez");

    SetTargetFPS(60);

    // carrega a matriz com as peças do jogo
    Peca **tabuleiro = iniciar_tabuleiro();

    Peca **pecas_conquistadas = iniciar_array_pecas();

    // carrega as imagens das peças
    Texture2D **imagens = carregar_imagens();

    if(tabuleiro == NULL || imagens == NULL){
        printf("ERRO de alocação\n");
        return 1;
    }

    Vector2 peca_ativa = (Vector2){NULA, NULA};
    Vector2 peca_ativa_anteriormente;

    // Debug
    tabuleiro[5][0] = (Peca){TORRE, PRETO};

    while(!WindowShouldClose()){

        BeginDrawing();

        ClearBackground((Color){222, 184, 135, 1});

        DesenharTabuleiro();

        DesenharPecas(imagens, tabuleiro);

        peca_ativa_anteriormente = peca_ativa;

        peca_ativa = verifica_peca_selecionada(tabuleiro, peca_ativa);

        DesenharPossiveisMovimentos(tabuleiro, peca_ativa);

        EndDrawing();
    }

    // Limpar memória para fechar o programa
    for(int i = 0; i < 8; i++){
        free(tabuleiro[i]);
    }
    
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 6; j++){
            UnloadTexture(imagens[i][j]);
        }
        free(imagens[i]);
        free(pecas_conquistadas[i]);
    }

    free(imagens);
    free(tabuleiro);
    free(pecas_conquistadas);

    CloseWindow();
}