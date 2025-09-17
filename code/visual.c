#include "raylib.h"
#include "estruturas.h"
#include "logica.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

const int lado_tabuleiro = SH * 0.8;
const int lado_quadrado = lado_tabuleiro / 8;
const int fonte = lado_quadrado * 0.5;

const Vector2 inicio = {(SW - lado_tabuleiro) / 2, (SH - lado_tabuleiro) / 2};

void DesenharTabuleiro(){

    Color branco = RAYWHITE;
    Color preto = COR_PRETO;

    Vector2 mouse = GetMousePosition();

    char letras[] = "abcdefgh";

    // desenha os quadrados do tabuleiro, alternando de cor
    for(int linha = 0; linha < 8; linha++){
        for(int coluna = 0; coluna < 8; coluna++){

            Color cor = ((linha + coluna) % 2 == 0) ? branco : preto;

            Rectangle quadrado = (Rectangle){inicio.x + lado_quadrado * coluna, inicio.y + lado_quadrado * linha, lado_quadrado, lado_quadrado};

            DrawRectangleRec(quadrado, cor);

            // desenha um destaque vermelhor em volta do quadrado sob o ponteiro do mouse
            if(CheckCollisionPointRec(mouse, quadrado)){
                DrawRectangleLinesEx(quadrado, lado_quadrado * 0.025, RED);

                DrawText(TextFormat("%c%d", letras[coluna], 8 - linha ), 
                (SW - MeasureText(TextFormat("%c%d", letras[coluna], 8 - linha), fonte)) / 2,
                (SH - lado_tabuleiro) / 2 - (lado_quadrado + fonte) / 2,
                fonte, BLACK);
            }
        }
    }

    // desenha as letras e números em volta do tabuleiro
    int letra_y = (SH + lado_tabuleiro + lado_quadrado - fonte ) / 2;

    for(int i = 0; i < 8; i++){

        int letra_x = (SW - lado_tabuleiro) / 2 + lado_quadrado * i + (lado_quadrado - MeasureText(TextFormat("%c", letras[i]), fonte)) / 2;

        DrawText(TextFormat("%c", letras[i]), letra_x, letra_y ,fonte, BLACK);

        int numero_y = (SH - lado_tabuleiro) / 2 + lado_tabuleiro - lado_quadrado * i - (lado_quadrado + fonte) / 2;
        int numero_x = (SW - lado_tabuleiro) / 2 - (lado_quadrado + MeasureText(TextFormat("%d", i + 1), fonte)) / 2;

        DrawText(TextFormat("%d", i + 1), numero_x, numero_y ,fonte, BLACK);

    }
}

void DesenharPecas(Texture2D **imagens, Peca **tabuleiro){

    Vector2 mouse = GetMousePosition();

    // desenha as peças do jogo
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            TipoPeca tipo = tabuleiro[i][j].tipo;

            // ignora as peças nulas
            if(tipo != NULA){

                Rectangle quadrado_atual = (Rectangle){inicio.x + lado_quadrado * j, inicio.y + lado_quadrado * i, lado_quadrado, lado_quadrado};

                int cor = (int)tabuleiro[i][j].cor;

                float escala = (float)lado_quadrado / imagens[cor][tipo].height;

                // se a peça estiver sob o ponteiro do mouse, ela autenta um pouco de tamanho
                if(CheckCollisionPointRec(mouse, quadrado_atual)){

                    quadrado_atual.x -= imagens[cor][tipo].width * escala * 0.025;
                    quadrado_atual.y -= imagens[cor][tipo].height * escala * 0.025;

                    escala *= 1.05;
                } 

                DrawTextureEx(imagens[cor][tipo], (Vector2){quadrado_atual.x, quadrado_atual.y}, 0.0, escala, WHITE);
            }
        }
    }
}

Vector2 verifica_peca_selecionada(Peca **tabuleiro, Vector2 atual){

    Vector2 mouse = GetMousePosition();

    // Verifica se o botão esquerdo foi clicado
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

        // Se sim, verifica qual casa foi clicada
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){

                Rectangle quadrado_atual = (Rectangle){inicio.x + lado_quadrado * j, inicio.y + lado_quadrado * i, lado_quadrado, lado_quadrado};

                if(CheckCollisionPointRec(mouse, quadrado_atual)){
                    // Se um quadrado foi clicado e ele tem alguma peça, é retornada sua coordenada no tabuleiro
                    return (Vector2){j,i};
                }
            }
        }

        // Caso seja clicado em uma casa vazia ou fora do tabuleiro, é retornado um vetor "nulo"
        return (Vector2){NULA, NULA};
    }

    // Caso não haja clickes, ele apenas desenha um retangulo azul e escreve sua localização no topo, se o vetor não for "nulo"
    if(atual.x != NULA){
        Rectangle quadrado_atual = (Rectangle){inicio.x + lado_quadrado * atual.x, inicio.y + lado_quadrado * atual.y, lado_quadrado, lado_quadrado};
        DrawRectangleLinesEx(quadrado_atual, lado_quadrado * 0.025, BLUE);      
    }
    
    return atual;
}

void DesenharPossiveisMovimentos(Peca **tabuleiro, Vector2 peca){

    if(peca.y == NULA){
        return;
    }
    if(tabuleiro[(int)peca.y][(int)peca.x].tipo == NULA){
        return;
    }

    bool **possiveis_movimentos = VerificarMovimentosPossiveis(tabuleiro, peca);

    if(possiveis_movimentos == NULL){
        printf("ERRO de alocação\n");
        return;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            // Debug
            printf("%d ", (int)possiveis_movimentos[i][j]);

            if(possiveis_movimentos[i][j]){
                DrawCircle(inicio.x + lado_quadrado * j + lado_quadrado / 2, inicio.y + lado_quadrado * i + lado_quadrado / 2, lado_quadrado / 4.0, GRAY);
            }
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; i < 8; i++){
        free(possiveis_movimentos[i]);
    }

    free(possiveis_movimentos);
}

