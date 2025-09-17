#include "raylib.h"
#include "headers.h"
#include "logica.h"
#include "visual.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const float col = SW / 16;
const float row_v = SH / 16;
float margin = row_v * 0.1;
float font_size_v = row_v * 1.5;

char * desenhar_botoes(Font fonte){

    char *texto;
    int qtd_pastas = contar_dir();
    float altura_botao = 0;
    static int pos = 0;

    if(qtd_pastas < 1){
        DrawTextEx(fonte, "Nenhuma pasta no diretorio \"questoes\" foi encontrada", (Vector2){(SW - MeasureTextEx(fonte, "Nenhuma pasta no diretorio \"questoes\" foi encontrada", font_size_v, 2).x) / 2, row_v * 8}, font_size_v , 2 ,RED);
        return NULL;
    }

    if(qtd_pastas <= 5){

        altura_botao = row_v * 5 / qtd_pastas;

        for(int i = 0; i < qtd_pastas; i++){

            Rectangle botao_size = {
                col * 4,
                row_v * 8 + altura_botao * i + margin,
                col * 8,
                altura_botao - margin
            };

            texto = ler_dir(i);

            if(botao(botao_size, 0.1, texto, BLACK, BLUE, RAYWHITE, fonte)){
                return texto;
            }
        }
    }
    else{
        int dir_atual = 0 + pos;

        for(int i = pos; i < 5 + pos; i++){

            Rectangle botao_size = {
                col * 4,
                row_v * 8 + row_v * (i - pos) + margin,
                col * 8,
                row_v - margin
            };

            if(pos > 0 && i == pos){
                if(botao(botao_size, 0.1, "---", BLACK, BLUE, RAYWHITE, fonte)){
                    pos--;
                    break;
                }
            }else if(i == 4 + pos && i + pos <= qtd_pastas){
                if(botao(botao_size, 0.1, "---", BLACK, BLUE, RAYWHITE, fonte)){
                    pos++;
                    break;
                }
            }else{
                texto = ler_dir(dir_atual);

                if(botao(botao_size, 0.1, texto, BLACK, BLUE, RAYWHITE, fonte)){
                    return texto;
                }

                dir_atual++;
            }
        }
    }

    return NULL;
}

// Cria um botão e verifica se foi clicado
bool botao(Rectangle retangulo, float margin, char *texto, Color externo, Color interno, Color texto_cor, Font fonte){

    float diferenca;
    bool clicado = false;
    Vector2 mouse = GetMousePosition();

    // Verifica se o mouse está acima do botão
    if(CheckCollisionPointRec(mouse, retangulo)){

        diferenca = retangulo.height * sin(GetTime() * 2) * 0.1;

        retangulo.x -= diferenca / 2;
        retangulo.y -= diferenca / 2;
        retangulo.height += diferenca;
        retangulo.width += diferenca;

        // Verifica se o mouse clicou no notão
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            clicado = true;
        }

    }

    // Desenha o botão
    DrawRectangleRec(retangulo, externo);

    diferenca = retangulo.height * margin;

    retangulo.x += diferenca / 2;
    retangulo.y += diferenca / 2;
    retangulo.height -= diferenca;
    retangulo.width -= diferenca;

    DrawRectangleRec(retangulo, interno);

    diferenca = retangulo.height * margin;

    retangulo.x += diferenca / 2;
    retangulo.y += diferenca ;
    retangulo.height -= diferenca;
    retangulo.width -= diferenca;

    // Vefica se o texto não vai ficar maior que o botão
    Vector2 largura = MeasureTextEx(fonte, texto, retangulo.height, 2);
    while(largura.x > retangulo.width){
        retangulo.height--;
        retangulo.y += 0.5;
        largura = MeasureTextEx(fonte, texto, retangulo.height, 2);
    }

    // Desenha o texto
    DrawTextEx(fonte, texto, (Vector2){((retangulo.width - MeasureTextEx(fonte, texto, retangulo.height, 2).x) / 2) + retangulo.x, retangulo.y}, retangulo.height, 2, texto_cor);

    return clicado;
}

// Calcula o x em que o texto deve ser desenhado para ficar centralizado
float centro_x_texto(char *texto, int centro, int tamanho_fonte, Font fonte){

    Vector2 largura_texto = MeasureTextEx(fonte, texto, tamanho_fonte, 2);
    return centro - largura_texto.x / 2;

}