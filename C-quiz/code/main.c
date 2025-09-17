#include "raylib.h"
#include "headers.h"
#include "logica.h"
#include "visual.h"
#include <stdlib.h>
#include <stdio.h>

telas menu();
void debug();

Font fonte;
const float row = SH / 8;
float font_size = row * 1.5;

char *pasta;

int main(){

    InitWindow(SW, SH, "C-Quiz");

    SetTargetFPS(60);

    Texture2D fundo = LoadTexture("assets/quadro.jpg");
    fundo.width = SW;
    fundo.height = SH;

    telas tela = MENU;

    fonte = carrega_fonte("assets/Chalk_Board.ttf", font_size);

    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureEx(fundo, (Vector2){0,0}, 0.0, 1.0, WHITE);

        //debug();

        switch(tela){
            default:
                tela = menu();
                break;
        }

        EndDrawing();
    }

    UnloadTexture(fundo);

    CloseWindow();
}

telas menu(){

    float margin = row * 0.1; 

    DrawTextEx(fonte, "Bem-vindo ao C-Quiz!", (Vector2){(SW - MeasureTextEx(fonte, "Bem-vindo ao C-Quiz!", font_size, 2).x) / 2, row * 2.5}, font_size , 2 ,RAYWHITE);

    pasta = desenhar_botoes(fonte);

    if(pasta != NULL){
        printf("%s\n", pasta);
    }

    return MENU;
}

void debug(){

    float qtd_barras = 16.0;

    for(float i = 0; i < SW; i += SW / qtd_barras){
        DrawRectangle(i, 0, SW * 0.005, SH, BLUE);
    }

    for(float i = 0; i < SH; i += SH / qtd_barras){
        DrawRectangle(0, i, SW, SH * 0.005, RED);
    }
}