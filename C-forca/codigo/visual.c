#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "logica.h"
#include "visual.h"

#define COR_BORDA_BOTAO (Color){30, 30, 30, 255} 
#define COR_BOTAO (Color){70, 130, 180, 255}
#define COR_TEXTO_BOTAO (Color){245, 245, 245, 255}

int jogo(char *string, Font fonte, Texture2D *forca, int tela){

    static int tamanho_fonte = 100;

    // Variáveis de referência para ajudar no possicionamento
    float coluna = SW * 0.1;
    float row = SH * 0.1;

    static int erros = 0;

    static char tentativas[1000] = "\n!\"#$%&'()*+,-./:;<=>?@ [\\]^_`{|}~";

    // Letra do teclado
    static char letra = '\0';

    static char mensagem[1000] = "";
    static Color cor_mensagem = GREEN;

    static bool tudo_achado;

    // Vai para as telas de teclado e de fim_jogo se selecionadas
    if(tela == TECLADO){
        letra = teclado(&tela, fonte, tentativas);
        printf("%c\n", letra);
        return tela;
    }

    if(tela == FIM_JOGO){
        return fim_jogo(string, fonte, forca, tudo_achado);
    }

    // Desenha a forca
    DrawTextureEx(*forca, (Vector2){coluna * 1.25 , row * 3}, 0.0, 0.65,  WHITE);

    // Deixa as palavras desconhecidas como _ e mostra as achadas
    char *impressao = palavra_impressao(string, tentativas, &tudo_achado);

    // Verificação de erros
    if(string == NULL || impressao == NULL){
        return MENU;
    }

    // Quebra palavras grandes em duas linhas ou diminui o texto
    if((int)MeasureTextEx(fonte, impressao, tamanho_fonte, 15).x > coluna * 5){

        if(letra_esta_string(impressao, ' ')){
            int len = strlen(impressao);

            for(int i = 0; i < len; i++){
                if(impressao[i] == ' '){
                    impressao[i] = '\n';
                    break;
                }
            }
        }else{
            while((int)MeasureTextEx(fonte, impressao, tamanho_fonte, 15).x > coluna * 5){
                tamanho_fonte--;
            }
        }
    } 

    // Desenha a palavra
    DrawTextEx(fonte, impressao, (Vector2){coluna * 3, row * 6.75}, tamanho_fonte, 15, WHITE);

    // Desenha as tentativas anteriores
    DrawTextEx(fonte, &tentativas[34], (Vector2){coluna * 3.5, row * 3}, 65, 20, YELLOW);

    // Botão para ir a tela teclado
    if(botao((Rectangle){coluna * 7.75, row * 0.25, coluna * 2, row * 1.50}, 0.1, "Teclado", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte) || tela == TECLADO){
        tela = TECLADO;
    }

    // Botão para resetar o jogo
    if(botao((Rectangle){coluna * 0.25, row * 0.25, coluna * 2, row * 1.50}, 0.1, "Resetar", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        tela = MENU;
        erros = 0; 

        UnloadTexture(*forca);
        *forca = LoadTexture("imagens/forca_0.png");

        sprintf(tentativas, "\n!\"#$%&'()*+,-./:;<=>?@ [\\]^_`{|}~");
        sprintf(mensagem, "");
        tamanho_fonte = 100;
    }

    // Libera a string impressão
    free(impressao);

    // Verifica se alguma letra foi escolhida no teclado
    if(letra != '\0'){

        if(letra_esta_string(&tentativas[34], letra)){

            sprintf(mensagem, "Voce ja fez essa tentaiva antes :|");
            cor_mensagem = YELLOW;

        }else{
            add_string(tentativas, letra);

            if(letra_esta_string(string, letra)){

                sprintf(mensagem, "Voce acertou :D");
                cor_mensagem = GREEN;

            }else{

                erros++;
                UnloadTexture(*forca);
                char caminho[100];
                sprintf(caminho, "imagens/forca_%d.png", erros);
                *forca = LoadTexture(caminho);

                sprintf(mensagem, "Voce errou :(");
                cor_mensagem = RED;
            }
        }

        letra = '\0';
    }

    // Escreve a mensagem na tela
    DrawTextEx(fonte, mensagem, (Vector2){centro_x_texto(mensagem, coluna * 6, 100, fonte), row * 5}, 100, 0.2, cor_mensagem);

    // Verifica se o jogo já acabou
    if(tudo_achado || erros >= 6){
        erros = 0; 
        sprintf(tentativas,  "\n!\"#$%&'()*+,-./:;<=>?@ [\\]^_`{|}~");
        sprintf(mensagem, "");
        tela = FIM_JOGO;
        tamanho_fonte = 100;
    }

    // Retorna a próxima tela
    return tela;
}

int fim_jogo(char *palavra, Font fonte, Texture2D *forca, bool acertou_tudo){

    int tamanho_fonte = 100;

    // Variáveis de referência para ajudar no possicionamento
    float coluna = SW * 0.1;
    float row = SH * 0.1;

    char mensagem[1000] = "";
    Color cor_mensagem;

    // Caso sair, reseta o desenho fa forca
    if(botao((Rectangle){coluna * 0.25, row * 0.25, coluna * 2, row * 1.50}, 0.1, "Resetar", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        UnloadTexture(*forca);
        *forca = LoadTexture("imagens/forca_0.png");
        return MENU;
    }

    // Desenha a forca
    DrawTextureEx(*forca, (Vector2){coluna * 1.25 , row * 3}, 0.0, 0.65,  WHITE);

    // Quebra palavras grandes em duas linhas ou diminui o texto
    if((int)MeasureTextEx(fonte, palavra, tamanho_fonte, 15).x > coluna * 5){

        if(letra_esta_string(palavra, ' ')){
            int len = strlen(palavra);

            for(int i = 0; i < len; i++){
                if(palavra[i] == ' '){
                    palavra[i] = '\n';
                    break;
                }
            }
        }else{
            while((int)MeasureTextEx(fonte, palavra, tamanho_fonte, 15).x > coluna * 5){
                tamanho_fonte--;
            }
        }
    } 

    // Desenha a palavra
    DrawTextEx(fonte, palavra, (Vector2){coluna * 3, row * 6.75}, 100, 15, WHITE);

    // Imprimi a mensagem final
    if(acertou_tudo){
        sprintf(mensagem, "Parabens, voce acertou tudo ;)");
        cor_mensagem = GREEN;
    }else{
        sprintf(mensagem, "Suas tentativas se esgotaram :O\nMais sorte da proxima vez");
        cor_mensagem = RED;
    }

    DrawTextEx(fonte, mensagem, (Vector2){centro_x_texto(mensagem, coluna * 6.25, 100, fonte), row * 3.5}, 100, 0.2, cor_mensagem);

    return FIM_JOGO;

}

char teclado(int *tela, Font fonte, char *tentativas){

    // Variáveis de referência para ajudar no possicionamento
    float coluna = SW * 0.1;
    float row = SH * 0.1;
    float margin = row * 0.1;

    *tela = TECLADO;

    // Desenha o botão de sair
    if(botao((Rectangle){coluna * 0.25, row * 0.25, coluna * 2, row * 1.50}, 0.1, "Voltar", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        *tela = FORCA;
    }

    // Desenha o teclado virtual
    char letras_maiusculas[] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    float pos_x = coluna * 2.5;
    float pos_y = row * 3;

    for(int i = 0; i < 26; i++){

        if(pos_x > coluna * 6.5){

            pos_x = coluna * 2.5;
            pos_y += row;

        }

        char letra_atual[] = {letras_maiusculas[i] , '\0'};

        Color cor_fundo;
        if(letra_esta_string(tentativas, letras_maiusculas[i])){
            cor_fundo = (Color){180, 180, 180, 255};
        }else{
            cor_fundo = COR_BOTAO;
        }

        if(botao((Rectangle){pos_x + margin, pos_y + margin, coluna * 0.8, row * 0.8}, 0.1, letra_atual, COR_BORDA_BOTAO, cor_fundo, COR_TEXTO_BOTAO, fonte)){
            *tela = FORCA;
            return letras_maiusculas[i];
        }

        pos_x += coluna;
    }

    return '\0';
}


Font carrega_fonte(char *caminho, int tamanho){

    // Carrega todas as letras possíveis da fonte
    int chars[] = {
    // Letras maiúsculas
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'Á','À','Â','Ã','É','Ê','Í','Ó','Ô','Õ','Ú','Ç',
    
    // Letras minúsculas
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    'á','à','â','ã','é','ê','í','ó','ô','õ','ú','ç',
    
    // Números
    '0','1','2','3','4','5','6','7','8','9',
    
    // Pontuação e símbolos ASCII comuns
    '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~',
    
    // Espaço
    ' '
    };
    int charCount = sizeof(chars)/sizeof(chars[0]);

    return LoadFontEx(caminho, tamanho, chars, charCount);
}

int menu(Font fonte, char **string){

    int dificuldade = 0;

    // Variáveis de referência para ajudar no possicionamento
    float coluna = SW * 0.1;
    float row = SH * 0.1;
    float margin = row * 0.1;

    // Escrita dos títulos
    DrawTextEx(fonte, "Bem-vindo ao C-Forca!", (Vector2){centro_x_texto("Bem-vindo ao C-Forca!", SW / 2, 128, fonte), row * 2.5 + margin}, 128 , 2 ,WHITE);

    DrawTextEx(fonte, "Escolha uma das dificuldades", (Vector2){centro_x_texto("Escolha uma das dificuldades", SW / 2, 96, fonte), row * 4 + margin}, 96 , 2 ,WHITE);

    // Botões para escolher a dificuldade
    if(botao((Rectangle){coluna * 2 + margin, row * 6 + margin, coluna * 2 - margin * 2, row * 2 - margin * 2}, 0.1, "Facil", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        dificuldade = 1;
    }
    if(botao((Rectangle){coluna * 4 + margin, row * 6 + margin, coluna * 2 - margin * 2, row * 2 - margin * 2}, 0.1, "Medio", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        dificuldade = 2;
    }
    if(botao((Rectangle){coluna * 6 + margin, row * 6 + margin, coluna * 2 - margin * 2, row * 2 - margin * 2}, 0.1, "Dificil", COR_BORDA_BOTAO, COR_BOTAO, COR_TEXTO_BOTAO, fonte)){
        dificuldade = 3;
    }

    // Chama a função para escolher a dificuldade e vai para o jogo
    if(dificuldade > 0){
        
        *string = escolher_palavra_aleatoria(dificuldade);

        if(*string != NULL){
            printf("%s\n", *string);
            return FORCA;
        }
    }

    return MENU;
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
    DrawTextEx(fonte, texto, (Vector2){centro_x_texto(texto, retangulo.x + retangulo.width / 2, retangulo.height, fonte) , retangulo.y}, retangulo.height, 2, texto_cor);

    return clicado;
}

// Calcula o x em que o texto deve ser desenhado para ficar centralizado
float centro_x_texto(char *texto, int centro, int tamanho_fonte, Font fonte){

    Vector2 largura_texto = MeasureTextEx(fonte, texto, tamanho_fonte, 2);
    return centro - largura_texto.x / 2;

}