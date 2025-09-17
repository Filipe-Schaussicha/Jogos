#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "visual.h"

// Variáveis globais
telas tela_atual = MENU;
bool mostrar_grades = false;

int main(){

    InitWindow(SW, SH, "Forca");

    SetTargetFPS(60);

    srand(time(NULL));

    char *palavra = NULL;

    // Carrega imagem de fundo e da forca
    Texture2D quadro = LoadTexture("imagens/quadro.jpg");

    Texture2D forca = LoadTexture("imagens/forca_0.png");

    // Carrega a fonte customizada
    Font fonte = carrega_fonte("fontes/Chalk_Board.ttf", 96);

    // Loop principal
    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha tela de fundo
        DrawTextureEx(quadro, (Vector2){0,0}, 0.0, (float)SW / quadro.width, WHITE);

        // Para Debug
        if(mostrar_grades){
            for(int i = 0; i < SW; i += SW * 0.1){
                DrawRectangle(i, 0, SW * 0.005, SH, BLUE);
            }

            for(int i = 0; i < SH; i += SH * 0.1){
                DrawRectangle(0, i, SW, SH * 0.005, RED);
            }
        }
        
        // Selecão de telas
        switch(tela_atual){

            case FORCA: case TECLADO: case FIM_JOGO:
                tela_atual = jogo(palavra, fonte, &forca, tela_atual);
                break;

            default:
                tela_atual = menu(fonte, &palavra);
                break;
        }

        EndDrawing();
    }

    free(palavra);
    UnloadFont(fonte);
    UnloadTexture(quadro);
    UnloadTexture(forca);
    CloseWindow();
}


