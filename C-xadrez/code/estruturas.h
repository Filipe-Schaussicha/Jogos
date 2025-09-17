#define SW 1280
#define SH 720

#define COR_PRETO (Color){100, 100, 100, 255}

typedef enum{

    NULA = -1,
    PEAO,
    CAVALO,
    BISPO,
    TORRE,
    DAMA,
    REI

}TipoPeca;

typedef enum{

    BRANCO = 0,
    PRETO = 1

}CorPeca;

typedef struct{

    TipoPeca tipo;
    bool cor;

}Peca;