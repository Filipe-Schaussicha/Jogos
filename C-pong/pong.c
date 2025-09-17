#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// Tamanho da tela
#define SW 1280
#define SH 720

int menu();
bool botao(Rectangle retangulo, float porsentagem_retangulo_interno, Color interno, Color externo, Color cor_texto, char *texto);
int jogo();
int opcoes();
int fim_jogo(int pontos_direita, int pontos_esquerda);
int jogabilidade();
void texto_centralizado(Vector2 centro, int altura, float porsentagem_margin, char* texto, Color cor);
int visual(int caso);

// Definição das variáveis gloabais
Vector2 velocidade_bola = {525, 1};
int velocidade_pedal = 380;
int max_pontuacao = 10;
const float tamanho_raquete = 0.1;
const float tamanho_bola = SH * 0.015;
bool mostrar_status = false;

// Paleta de cores inicial
Color bg = BLACK;
Color fg = WHITE; 
Color mgl = GRAY; 
Color mgd = DARKGRAY;

char nome_esquerda[] = "Player 1";
char nome_direita[] = "Player 2"; 

int main(){

    int opt = 0;

    srand(time(NULL));

    InitWindow(SW, SH, "C-Pong");

    SetTargetFPS(60);

    // Loop do jogo
    while(!WindowShouldClose()){

        BeginDrawing();

        ClearBackground(bg);

        // Escolhe qual janela será apresentada
        switch(opt){
            case 1:
                opt = jogo();
                break;
            case 2:
                opt = opcoes();
                break;
            case 3:
                EndDrawing();
                goto fim;
            case 4:
                opt = fim_jogo(-1,-1);
                break;
            case 5:
                opt = jogabilidade();
                break;
            case 6: case 8:
                opt = visual(opt);
                break;
            default:
                opt = menu();
                break;
        }
        
        EndDrawing();
    }

    fim:

    CloseWindow();
}

// Permite edcolher a palheta de cor que será usada
int visual(int caso){

    static Color *cor;

    int row = SH / 9;

    texto_centralizado((Vector2){SW / 2, row * 1.5}, row, 0.1, "Selecione uma paleta de cores:", fg);

    texto_centralizado((Vector2){SW / 4, row * 2.5}, row * 0.75, 0.1, "Preto e branco:", fg);
    texto_centralizado((Vector2){SW / 4, row * 3.5}, row * 0.75, 0.1, "Tons de Azul:", fg);
    texto_centralizado((Vector2){SW / 4, row * 4.5}, row * 0.75, 0.1, "Tons de verde:", fg);
    texto_centralizado((Vector2){SW / 4, row * 5.5}, row * 0.75, 0.1, "Moderno:", fg);
    texto_centralizado((Vector2){SW / 4, row * 6.5}, row * 0.75, 0.1, "Vintage:", fg);

    if(botao((Rectangle){SW * 3 /4, row * 2.5, SW * 0.3, row * 0.8}, 0.8, mgl, mgd,fg, "Selecionar")){
        bg = BLACK;
        fg = WHITE; 
        mgl = GRAY; 
        mgd = DARKGRAY;
    }
    if(botao((Rectangle){SW * 3/ 4, row * 3.5, SW * 0.3, row * 0.8}, 0.8, mgl, mgd,fg, "Selecionar")){
        bg = (Color){2, 62, 138, 255};
        fg = (Color){202, 240, 248, 255}; 
        mgl = (Color){72, 202, 228, 255}; 
        mgd = (Color){0, 119, 182, 255};
    }
    if(botao((Rectangle){SW * 3/ 4, row * 4.5, SW * 0.3, row * 0.8}, 0.8, mgl, mgd,fg, "Selecionar")){
        bg = (Color){40, 54, 24, 255};
        fg = (Color){254, 250, 224, 255}; 
        mgl = (Color){221, 161, 94, 255}; 
        mgd = (Color){96, 108, 56, 255};
    }
    if(botao((Rectangle){SW * 3/ 4, row * 5.5, SW * 0.3, row * 0.8}, 0.8, mgl, mgd,fg, "Selecionar")){
        bg = (Color){40, 75, 99, 255};
        fg = (Color){255, 255, 255, 255}; 
        mgl = (Color){217, 217, 217, 255}; 
        mgd = (Color){60, 110, 113, 255};
    }
    if(botao((Rectangle){SW * 3/ 4, row * 6.5, SW * 0.3, row * 0.8}, 0.8, mgl, mgd,fg, "Selecionar")){
        bg = (Color){48, 76, 137, 255};
        fg = (Color){232, 229, 218, 255}; 
        mgl = (Color){158, 183, 229, 255}; 
        mgd = (Color){205, 195, 146, 255};
    }

    if(botao((Rectangle){SW / 2, row * 7.5, SW / 2, row * 0.8}, 0.8, mgl, mgd, fg, "<- Voltar")){
        return 2;
    }

    return caso;

}

// Escreve texto de forma centralizada
void texto_centralizado(Vector2 centro, int altura, float porsentagem_margin, char* texto, Color cor){

    centro.x -= SW / 2;

    int tamanho_fonte = altura * (1 - porsentagem_margin * 2);
    int x = ((SW - MeasureText(texto, tamanho_fonte)) / 2) + centro.x;
    int y = centro.y - altura / 2;

    DrawText(texto, x, y, tamanho_fonte, cor);

}

// Menu de opções de jogabilidade
int jogabilidade(){

    int row = SH / 7;
    char buffer[1000];

    int magin_lateral = SW * 0.05;
    int texto_lateral = SW * 0.45;
    int valor_lateral = SW * 0.2;
    int botao_lateral = SW * 0.1;

    sprintf(buffer, "%.0f", velocidade_bola.x);
    texto_centralizado((Vector2){texto_lateral / 2 + magin_lateral, row * 1.5}, row * 0.625, 0.1, "Velocidade da bola:", fg);
    texto_centralizado((Vector2){magin_lateral + texto_lateral + botao_lateral + valor_lateral / 2, row * 1.5}, row, 0.1, buffer, fg);
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral / 2, row * 1.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "-")){
        velocidade_bola.x--;
    }
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral  + valor_lateral + botao_lateral / 2, row * 1.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "+")){
        velocidade_bola.x++;
    }

    sprintf(buffer, "%d", velocidade_pedal);
    texto_centralizado((Vector2){texto_lateral / 2 + magin_lateral, row * 2.5}, row * 0.625, 0.1, "Velocidade do pedal:", fg);
    texto_centralizado((Vector2){magin_lateral + texto_lateral + botao_lateral + valor_lateral / 2, row * 2.5}, row, 0.1, buffer, fg);
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral / 2, row * 2.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "-")){
        velocidade_pedal--;
    }
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral  + valor_lateral + botao_lateral / 2, row * 2.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "+")){
        velocidade_pedal++;
    }

    sprintf(buffer, "%d", max_pontuacao);
    texto_centralizado((Vector2){texto_lateral / 2 + magin_lateral, row * 3.5}, row * 0.625, 0.1, "Pontuação Máxima:", fg);
    texto_centralizado((Vector2){magin_lateral + texto_lateral + botao_lateral + valor_lateral / 2, row * 3.5}, row, 0.1, buffer, fg);
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral / 2, row * 3.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "-") && max_pontuacao > 1){
        max_pontuacao--;
    }
    if(botao((Rectangle){magin_lateral + texto_lateral + botao_lateral  + valor_lateral + botao_lateral / 2, row * 3.5, botao_lateral * 0.8, row * 0.8}, 0.8, mgl, mgd, fg, "+")){
        max_pontuacao++;
    }

    texto_centralizado((Vector2){texto_lateral / 2 + magin_lateral, row * 4.5}, row * 0.625, 0.1, "Mostrar FPS:", fg);
    if(mostrar_status){
        if(botao((Rectangle){magin_lateral + texto_lateral + valor_lateral, row * 4.5, SW * 0.4 * 0.8, row * 0.8}, 0.8, RED, mgl, fg, "Desativar"))
            mostrar_status = false;
    }else{
        if(botao((Rectangle){magin_lateral + texto_lateral + valor_lateral, row * 4.5, SW * 0.4 * 0.8, row * 0.8}, 0.8, GREEN, mgl, fg, "Ativar"))
            mostrar_status = true;
    }
    

    if(botao((Rectangle){SW / 2, row * 5.5, SW / 2, row * 0.8}, 0.8, mgl, mgd, fg, "<- Voltar")){
        return 2;
    }

    return 5;
}

// Menu mostrado no fim de cada partida
int fim_jogo(int pontos_direita, int pontos_esquerda){

    static int d = 0, e = 0;

    if(pontos_direita > 0 || pontos_esquerda > 0){
        d = pontos_direita;
        e = pontos_esquerda;
    }

    int row = SH / 6;
    
    DrawText("O ganhador é:", (SW - MeasureText("O ganhador é:", row * 0.8)) / 2, row * 1.1, row * 0.8, fg);
    if(d > e){
        DrawText(nome_direita, (SW - MeasureText(nome_direita, row * 0.8)) / 2, row * 2.1, row * 0.8, BLUE);
    }else{
        DrawText(nome_esquerda, (SW - MeasureText(nome_esquerda, row * 0.8)) / 2, row * 2.1, row * 0.8, RED);
    }

    if(botao((Rectangle){SW / 2, row * 4.5, SW / 2, row * 0.8}, 0.8, mgl, mgd, fg, "Nova partida")){
        return 1;
    }

    return 4;
}

// Menu de opções
int opcoes(){

    int tamanho_faixa = SH / 6;
    int padding_vertical = tamanho_faixa * 0.15;
    int padding_horizontal = SW * 0.1;
    int tamanho_fonte = tamanho_faixa * 0.75;

    DrawText("OPÇÕES:", (SW - MeasureText("OPÇÕES:", tamanho_fonte)) / 2, tamanho_faixa + padding_vertical, tamanho_fonte, fg);

    if(botao((Rectangle){SW / 2, tamanho_faixa * 2.5, SW / 3, tamanho_fonte}, 0.8, mgl, mgd, fg, "Jogabilidade")){
        return 5;
    }
    if(botao((Rectangle){SW / 2, tamanho_faixa * 3.5, SW / 3, tamanho_fonte}, 0.8, mgl, mgd, fg, "Visual")){
        return 6;
    }
    if(botao((Rectangle){SW / 2, tamanho_faixa * 4.5, SW / 3, tamanho_fonte}, 0.8, mgl, mgd, fg, "<- Voltar")){
        return 0;
    }

    return 2;
}

// O jogo de pong em si
int jogo(){

    static bool primeira_chamada = true;
    int saida = 1;

    // Decide de forma aleatória a velocidade y da bola quando sai
    if(primeira_chamada){
        velocidade_bola.y = (rand() % (int)(abs(velocidade_bola.x) * 2)) - abs(velocidade_bola.x);
    }

    // Define as bordas
    Rectangle borda_cima = {0, 0, SW, SH * 0.05};
    Rectangle borda_baixo = {0, SH * 0.95, SW, SH * 0.05};

    // Criar uma versão mais clara da cor mgl para as bordas
    float factor = 1.5;
    int r = mgl.r * factor;
    int g = mgl.g * factor;
    int b = mgl.b * factor;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    Color mgl_maisclaro = (Color){r, g, b, mgl.a};

    // Desenha as bordas
    DrawRectangleRec(borda_cima, mgl_maisclaro);
    DrawRectangleRec(borda_baixo, mgl_maisclaro);

    // Desenha o pontilhado central
    for(int i = SH * 0.05, j = 0; i < SH * 0.95; i += SH * 0.025, j++){
        if(j % 2 == 0) DrawRectangle(SW * 0.495, i, SW * 0.01, SH * 0.025, mgd);
    }

    // Define e desenha os pedais de ambos players
    static Rectangle player_direita = {SW * 0.96, SH * (0.5 - tamanho_raquete / 2), SW * 0.02, SH * tamanho_raquete};
    static int player_direita_pontos = 0;
    Vector2 centro_player_direita = {player_direita.x + player_direita.width / 2, player_direita.y + player_direita.height / 2};

    static Rectangle player_esquerda = {SW * 0.03, SH * (0.5 - tamanho_raquete / 2), SW * 0.02, SH * tamanho_raquete};
    static int player_esquerda_pontos = 0;
    Vector2 centro_player_esquerda = {player_esquerda.x + player_esquerda.width / 2, player_esquerda.y + player_esquerda.height / 2};

    DrawRectangleRec(player_direita, fg);
    DrawRectangleRec(player_esquerda, fg);

    // Escreve a pontuação na tela
    char buffer[50];
    sprintf(buffer, "%0.2d", player_esquerda_pontos);
    DrawText(buffer, SW * 0.45 - (SH * 0.05 * 0.5), SH * 0.075, SH * 0.05, WHITE);
    
    sprintf(buffer, "%0.2d", player_direita_pontos);
    DrawText(buffer, SW * 0.55 - (SH * 0.05 * 0.5), SH * 0.075, SH * 0.05, WHITE);

    // Escreve os nome dos players 1 e 2
    DrawText(nome_esquerda, (SW - MeasureText(nome_esquerda, SH * 0.03)) / 4, SH * 0.01, SH * 0.03, bg);
    DrawText(nome_direita, (SW - MeasureText(nome_direita, SH * 0.03)) * 3 / 4, SH * 0.01, SH * 0.03, bg);

    // Cria e desenha a bola
    static Vector2 bola = {SW / 2, SH / 2};

    DrawCircleV(bola, tamanho_bola, fg);

    // Movimenta a bola
    bola.x += velocidade_bola.x * GetFrameTime();
    bola.y += velocidade_bola.y * GetFrameTime();
    
    // Verifica se a bola saiu da tela
    if(bola.x > SW){
        player_esquerda_pontos++;
        bola.x = SW / 2;
        bola.y = SH / 2;
        velocidade_bola.x *= -1;
        velocidade_bola.y = (rand() % (int)(abs(velocidade_bola.x) * 2)) - abs(velocidade_bola.x);
    } 
    else if(bola.x < 0){
        player_direita_pontos++;
        bola.x = SW / 2;
        bola.y = SH / 2;
        velocidade_bola.x *= -1;
        velocidade_bola.y = (rand() % (int)(abs(velocidade_bola.x) * 2)) - abs(velocidade_bola.x);
    }

    // Lê entrada de usuário e move o pedal
    if(IsKeyDown(KEY_W) && player_esquerda.y > SH * 0.05){
        player_esquerda.y -= velocidade_pedal * GetFrameTime();
    }
    if(IsKeyDown(KEY_S) && player_esquerda.y < SH * 0.95 - player_direita.height){
        player_esquerda.y += velocidade_pedal * GetFrameTime();
    }
    if(IsKeyDown(KEY_UP) && player_direita.y > SH * 0.05){
        player_direita.y -= velocidade_pedal * GetFrameTime();
    }
    if(IsKeyDown(KEY_DOWN) && player_direita.y < SH * 0.95 - player_esquerda.height){
        player_direita.y += velocidade_pedal * GetFrameTime();
    }

    // Verifica de a bola colidiu com o pedal
    static float seno = 0;
    if(CheckCollisionCircleRec(bola, tamanho_bola, player_direita)){

        if(bola.y < player_direita.y) bola.y = player_direita.y - tamanho_bola - 1;
        if(bola.y > player_direita.y + player_direita.height)
        bola.y = player_direita.y + player_direita.height + tamanho_bola + 1;

        velocidade_bola.x *= -1;
        bola.x = player_direita.x - tamanho_bola  - 1;

        float hipotenusa = sqrt(pow(bola.x - centro_player_direita.x, 2) + pow(bola.y - centro_player_direita.y, 2));
        seno = (bola.y - centro_player_direita.y) / hipotenusa;

        if(velocidade_bola.y / seno < 0){
            seno *= -1;
        }

        velocidade_bola.y = abs(velocidade_bola.x) * seno;
        
    }
    else if(CheckCollisionCircleRec(bola, tamanho_bola, player_esquerda)){

        if(bola.y < player_esquerda.y) bola.y = player_esquerda.y - tamanho_bola - 1;
        if(bola.y > player_esquerda.y + player_esquerda.height)
        bola.y = player_esquerda.y + player_esquerda.height + tamanho_bola + 1;
        
        float hipotenusa = sqrt(pow(bola.x - centro_player_esquerda.x, 2) + pow(bola.y - centro_player_esquerda.y, 2));
        seno = (bola.y - centro_player_esquerda.y) / hipotenusa;

        if(velocidade_bola.y / seno < 0){
            seno *= -1;
        }

        velocidade_bola.y = abs(velocidade_bola.x) * seno;
        
        velocidade_bola.x *= -1;
        bola.x = player_esquerda.x + player_esquerda.width + tamanho_bola + 1;
        
    }

    // Verifica de a bola colidiu com as bordas
    if(CheckCollisionCircleRec(bola, tamanho_bola, borda_cima) || CheckCollisionCircleRec(bola, tamanho_bola, borda_baixo)){
        velocidade_bola.y *= -1;
    }

    // Usado mais em cima
    primeira_chamada = false;

    // Mostra os status como o FPS
    if(mostrar_status){

        Color cor_fps = GREEN;

        int fps = GetFPS();

        if(fps < 30){
            cor_fps = RED;
        }else if (fps < 45){
            cor_fps = YELLOW;
        }else{
            cor_fps = GREEN; 
        }

        sprintf(buffer, "Velocidade x: %.2f\nVelocidade Y: %.2f", velocidade_bola.x, velocidade_bola.y);
        DrawText(buffer, 50, 50, 25, RED);

        sprintf(buffer, "\n\nFPS: %d", fps);
        DrawText(buffer, 50, 50, 25, cor_fps);
    }

    // Botões de voltar e resetar
    if(botao((Rectangle){SW * 0.25, SH * 0.975, SW * 0.3, SH * 0.03}, 0.9, mgl, mgd, fg, "<- Voltar")){
        saida = 0;
    }

    bool resetar = botao((Rectangle){SW * 0.75, SH * 0.975, SW * 0.3, SH * 0.03}, 0.9, mgl, mgd, fg, "Restar o Jogo");

    // Recomeça o jogo
    if(resetar || player_direita_pontos >= max_pontuacao || player_esquerda_pontos >= max_pontuacao){
        
        // Caso alguém tenha ganahdo, vai para a tela de fim de jogo
        if(!resetar){
            saida = fim_jogo(player_direita_pontos, player_esquerda_pontos);
        }

        player_direita_pontos = 0;
        player_esquerda_pontos = 0;
        bola.x = SW / 2;
        bola.y = SH / 2;
        velocidade_bola.y = (rand() % (int)(abs(velocidade_bola.x) * 2)) - abs(velocidade_bola.x);
        
    }

    // Retorna a próxima tela a ser mostrada
    return saida;
}

// Menu principal
int menu(){

    int opt = 0;
    int row = SH / 6;
    int padding = row * 0.1;
    int content = row * 0.8;

    DrawText("Bem vindo ao C-Pong!", 150, row - padding + padding * sin(GetTime() * 2.5), content, fg);

    if(botao((Rectangle){SW / 2, row * 2.5, SW * 0.5, content}, 0.8, mgl, mgd, fg, "Jogar")) opt = 1;

    if(botao((Rectangle){SW / 2, row * 3.5, SW * 0.5, content}, 0.8, mgl, mgd, fg, "Opções")) opt =  2;

    if(botao((Rectangle){SW / 2, row * 4.5, SW * 0.5, content}, 0.8, mgl, mgd, fg, "Sair")) opt =  3;

    DrawText("Feito por Filipe S.", SW - MeasureText("Feito por Filipe S.", row * 0.25) - SW * 0.025, SH * 0.975 - row * 0.25, row * 0.25, fg);

    return opt;

}

// Cria um botão e detecta se ele já foi clicado
bool botao(Rectangle retangulo, float porsentagem_retangulo_interno, Color interno, Color externo, Color cor_texto, char *texto){

    Rectangle rec_externo = {retangulo.x - retangulo.width / 2, retangulo.y - retangulo.height / 2, retangulo.width, retangulo.height};

    Vector2 ponteiro_mouse = GetMousePosition();

    bool pressionado = false;

    if(CheckCollisionPointRec(ponteiro_mouse, rec_externo)){
        retangulo.width *= 1.2;
        retangulo.height *= 1.2;

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            pressionado = true;
        }
    }

    DrawRectangle(retangulo.x - retangulo.width / 2, retangulo.y - retangulo.height / 2, retangulo.width, retangulo.height, externo);

    int altura_original = retangulo.height;
    retangulo.height *= porsentagem_retangulo_interno;
    retangulo.width -= (altura_original - retangulo.height);

    DrawRectangle(retangulo.x - retangulo.width / 2, retangulo.y - retangulo.height / 2, retangulo.width, retangulo.height, interno);

    altura_original = retangulo.height;
    retangulo.height *= porsentagem_retangulo_interno;
    
    int pos_texto = (retangulo.height / 4) * strlen(texto);

    DrawText(texto, retangulo.x - pos_texto, retangulo.y - retangulo.height / 2, retangulo.height, cor_texto);

    return pressionado;

}