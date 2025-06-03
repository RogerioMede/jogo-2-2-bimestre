#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>
#include <windows.h>

#define MAX_LIN 50
#define MAX_COL 50

// Símbolos usados no mapa
#define SIM_JOGADOR        '&'
#define SIM_NPC            'P'
#define SIM_PAREDE         '*'
#define SIM_CHAVE          '@'
#define SIM_PORTA_FECHADA  'D'
#define SIM_PORTA_ABERTA   '='
#define SIM_BOTAO          'O'
#define SIM_ESPINHO        '#'
#define SIM_TELEPORTE      '>'
#define SIM_MONSTRO1       'X'
#define SIM_MONSTRO2       'V'
#define SIM_VAZIO          ' '

// Teclas de controle
#define TECLA_CIMA         'w'
#define TECLA_ESQUERDA     'a'
#define TECLA_BAIXO        's'
#define TECLA_DIREITA      'd'
#define TECLA_INTERAGE     'i'

// Pause no Windows
#define PAUSA() system("pause")

typedef enum {
    TELA_MENU,
    TELA_CREDITOS,
    TELA_VILA,
    TELA_FASE1,
    TELA_FASE2,
    TELA_FASE3,
    TELA_VITORIA,
    TELA_SAIR
} Estado;

typedef struct { int linha, coluna; } Posicao;

typedef struct {
    int nlin, ncol;
    char mapa[MAX_LIN][MAX_COL];
    Posicao jogador;
    Posicao chave;
    Posicao porta;
    Posicao npcs[2];
    int num_npcs;
    Posicao botao;
    Posicao espinhos[100];
    int num_espinhos;
    Posicao monstros1[10];
    int num_monstros1;
    Posicao monstros2[10];
    int num_monstros2;
    Posicao tele1, tele2;
    bool porta_aberta;
    bool chave_pega;
    int tentativas;
} Fase;

Fase fase_vila, fase1, fase2, fase3;
Estado estado = TELA_MENU;

// Protótipos
void carrega_mapas();
void show_menu();
void show_creditos();
void show_vitoria();
void jogo_vila();
void jogo_fase1();
void jogo_fase2();
void jogo_fase3();
void desenha_tela(Fase *f);
void processa_input(Fase *f, char tecla);
void reinicia_fase(Fase *f, int fase_num);
void atualiza_monstros1(Fase *f);
void atualiza_monstros2(Fase *f);
void teleporta(Fase *f, int lin, int col);

int main() {
    srand((unsigned)time(NULL));
    carrega_mapas();
    while (estado != TELA_SAIR) {
        switch (estado) {
            case TELA_MENU:     show_menu();     break;
            case TELA_CREDITOS: show_creditos(); break;
            case TELA_VILA:     jogo_vila();     break;
            case TELA_FASE1:    jogo_fase1();    break;
            case TELA_FASE2:    jogo_fase2();    break;
            case TELA_FASE3:    jogo_fase3();    break;
            case TELA_VITORIA:  show_vitoria();  break;
            default:            estado = TELA_MENU; break;
        }
    }
    return 0;
}

void carrega_mapas() {
    // ===== VILA (10×10) =====
    fase_vila.nlin = 10; fase_vila.ncol = 10;
    fase_vila.num_npcs = 2;
    fase_vila.num_espinhos = 0;
    fase_vila.num_monstros1 = 0;
    fase_vila.num_monstros2 = 0;
    fase_vila.porta_aberta = false;
    fase_vila.chave_pega = false;
    fase_vila.tentativas = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fase_vila.mapa[i][j] = (i == 0 || i == 9 || j == 0 || j == 9)
                                  ? SIM_PAREDE : SIM_VAZIO;

    fase_vila.jogador.linha = 1; fase_vila.jogador.coluna = 1;
    fase_vila.mapa[1][1] = SIM_JOGADOR;

    fase_vila.npcs[0].linha = 1;  fase_vila.npcs[0].coluna = 8;
    fase_vila.npcs[1].linha = 8;  fase_vila.npcs[1].coluna = 1;
    fase_vila.mapa[1][8] = SIM_NPC;
    fase_vila.mapa[8][1] = SIM_NPC;

    fase_vila.chave.linha = 2;  fase_vila.chave.coluna = 2;
    fase_vila.mapa[2][2] = SIM_CHAVE;

    fase_vila.porta.linha = 5;  fase_vila.porta.coluna = 9;
    fase_vila.mapa[5][9] = SIM_PORTA_FECHADA;

    // ===== FASE 1 (10×10) =====
    fase1.nlin = 10; fase1.ncol = 10;
    fase1.num_npcs = 0;
    fase1.num_espinhos = 0;
    fase1.num_monstros1 = 0;
    fase1.num_monstros2 = 0;
    fase1.porta_aberta = false;
    fase1.chave_pega = false;
    fase1.tentativas = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            fase1.mapa[i][j] = (i == 0 || i == 9 || j == 0 || j == 9)
                                ? SIM_PAREDE : SIM_VAZIO;

    fase1.jogador.linha = 1; fase1.jogador.coluna = 1;
    fase1.mapa[1][1] = SIM_JOGADOR;

    fase1.chave.linha = 2;  fase1.chave.coluna = 2;
    fase1.mapa[2][2] = SIM_CHAVE;

    fase1.porta.linha = 8;  fase1.porta.coluna = 8;
    fase1.mapa[8][8] = SIM_PORTA_FECHADA;

    // ===== FASE 2 (20×20) =====
    fase2.nlin = 20; fase2.ncol = 20;
    fase2.num_npcs = 0;
    fase2.num_espinhos = 5;
    fase2.num_monstros1 = 2;
    fase2.num_monstros2 = 0;
    fase2.porta_aberta = false;
    fase2.chave_pega = false;
    fase2.tentativas = 0;
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            fase2.mapa[i][j] = (i == 0 || i == 19 || j == 0 || j == 19)
                                ? SIM_PAREDE : SIM_VAZIO;

    fase2.jogador.linha = 1; fase2.jogador.coluna = 1;
    fase2.mapa[1][1] = SIM_JOGADOR;

    fase2.chave.linha = 10;  fase2.chave.coluna = 10;
    fase2.mapa[10][10] = SIM_CHAVE;

    fase2.porta.linha = 18;  fase2.porta.coluna = 18;
    fase2.mapa[18][18] = SIM_PORTA_FECHADA;

    fase2.botao.linha = 5;   fase2.botao.coluna = 5;
    fase2.mapa[5][5] = SIM_BOTAO;

    {
        int coords2[5][2] = {{3,3},{3,4},{4,3},{12,15},{7,12}};
        for (int i = 0; i < 5; i++) {
            fase2.espinhos[i].linha = coords2[i][0];
            fase2.espinhos[i].coluna = coords2[i][1];
            fase2.mapa[coords2[i][0]][coords2[i][1]] = SIM_ESPINHO;
        }
    }
    fase2.num_espinhos = 5;

    fase2.monstros1[0].linha = 2;   fase2.monstros1[0].coluna = 17;
    fase2.monstros1[1].linha = 17;  fase2.monstros1[1].coluna = 2;
    fase2.num_monstros1 = 2;
    fase2.mapa[2][17] = SIM_MONSTRO1;
    fase2.mapa[17][2] = SIM_MONSTRO1;

    for (int j = 5; j < 15; j++) {
        fase2.mapa[8][j] = SIM_PAREDE;
        fase2.mapa[12][j] = SIM_PAREDE;
    }

    // ===== FASE 3 (40×40) =====
    fase3.nlin = 40; fase3.ncol = 40;
    fase3.num_npcs = 0;
    fase3.num_espinhos = 10;
    fase3.num_monstros1 = 3;
    fase3.num_monstros2 = 1;
    fase3.porta_aberta = false;
    fase3.chave_pega = false;
    fase3.tentativas = 0;
    for (int i = 0; i < 40; i++)
        for (int j = 0; j < 40; j++)
            fase3.mapa[i][j] = (i == 0 || i == 39 || j == 0 || j == 39)
                                ? SIM_PAREDE : SIM_VAZIO;

    fase3.jogador.linha = 1; fase3.jogador.coluna = 1;
    fase3.mapa[1][1] = SIM_JOGADOR;

    fase3.chave.linha = 20;  fase3.chave.coluna = 20;
    fase3.mapa[20][20] = SIM_CHAVE;

    fase3.porta.linha = 38;  fase3.porta.coluna = 38;
    fase3.mapa[38][38] = SIM_PORTA_FECHADA;

    fase3.botao.linha = 10;  fase3.botao.coluna = 10;
    fase3.mapa[10][10] = SIM_BOTAO;

    {
        int coords3[10][2] = {{5,5},{5,6},{6,5},{15,15},{15,16},
                              {16,15},{25,25},{25,26},{26,25},{30,20}};
        for (int i = 0; i < 10; i++) {
            fase3.espinhos[i].linha = coords3[i][0];
            fase3.espinhos[i].coluna = coords3[i][1];
            fase3.mapa[coords3[i][0]][coords3[i][1]] = SIM_ESPINHO;
        }
    }
    fase3.num_espinhos = 10;

    fase3.monstros1[0].linha = 2;    fase3.monstros1[0].coluna = 38;
    fase3.monstros1[1].linha = 38;   fase3.monstros1[1].coluna = 2;
    fase3.monstros1[2].linha = 20;   fase3.monstros1[2].coluna = 1;
    fase3.num_monstros1 = 3;
    fase3.mapa[2][38] = SIM_MONSTRO1;
    fase3.mapa[38][2] = SIM_MONSTRO1;
    fase3.mapa[20][1] = SIM_MONSTRO1;

    fase3.monstros2[0].linha = 1;    fase3.monstros2[0].coluna = 38;
    fase3.num_monstros2 = 1;
    fase3.mapa[1][38] = SIM_MONSTRO2;

    fase3.tele1.linha = 12; fase3.tele1.coluna = 12;
    fase3.tele2.linha = 35; fase3.tele2.coluna = 35;
    fase3.mapa[12][12] = SIM_TELEPORTE;
    fase3.mapa[35][35] = SIM_TELEPORTE;

    for (int i = 5; i < 35; i++) {
        fase3.mapa[18][i] = SIM_PAREDE;
        fase3.mapa[i][18] = SIM_PAREDE;
    }
}

void show_menu() {
    char opcao;
    do {
        system("cls");
        printf("===== Dungeon Crawler =====\n");
        printf("1. Jogar\n");
        printf("2. Creditos\n");
        printf("3. Sair\n");
        printf("===========================\n");
        printf("Escolha uma opcao: ");
        opcao = getch();
        printf("%c\n", opcao);
        opcao = tolower(opcao);
        if (opcao == '1') { estado = TELA_VILA; return; }
        if (opcao == '2') { estado = TELA_CREDITOS; return; }
        if (opcao == '3') { estado = TELA_SAIR; return; }
    } while (1);
}

void show_creditos() {
    system("cls");
    printf("Rogerio Medeiros Carvalho Filho\n\n");
    printf("Pressione qualquer tecla para voltar ao menu.\n");
    getch();
    estado = TELA_MENU;
}

void show_vitoria() {
    system("cls");
    printf("Voce venceu! Parabens!\n");
    PAUSA();
    estado = TELA_MENU;
}

void desenha_tela(Fase *f) {
    system("cls");
    for (int i = 0; i < f->nlin; i++) {
        for (int j = 0; j < f->ncol; j++) {
            printf("%c", f->mapa[i][j]);
        }
        printf("\n");
    }
    printf("\nUse W/A/S/D para mover, I para interagir.\n");
    printf("Tentativas nesta fase: %d/3\n", f->tentativas);
}

void processa_input(Fase *f, char tecla) {
    tecla = tolower(tecla);
    int nova_lin = f->jogador.linha, nova_col = f->jogador.coluna;

    // Interacao (NPCs e botao)
    if (tecla == TECLA_INTERAGE) {
        if (f == &fase_vila) {
            for (int i = 0; i < fase_vila.num_npcs; i++) {
                int npc_lin = fase_vila.npcs[i].linha,
                    npc_col = fase_vila.npcs[i].coluna;
                if (abs(f->jogador.linha - npc_lin) +
                    abs(f->jogador.coluna - npc_col) == 1) {
                    if (i == 0) {
                        printf("NPC 1: Use W/A/S/D para mover o jogador (%c).\n", SIM_JOGADOR);
                        printf("Cada tecla move em uma direcao.\n");
                    } else {
                        printf("NPC 2: Colete a chave (%c) para abrir a porta (%c).\n",
                               SIM_CHAVE, SIM_PORTA_FECHADA);
                    }
                    PAUSA();
                    return;
                }
            }
        }
        if ((f == &fase2 || f == &fase3) &&
            f->jogador.linha == f->botao.linha &&
            f->jogador.coluna == f->botao.coluna) {
            printf("Botao pressionado! Novos espinhos aparecerao.\n");
            PAUSA();
            for (int k = 0; k < 2; k++) {
                int rlin = rand() % (f->nlin - 2) + 1;
                int rcol = rand() % (f->ncol - 2) + 1;
                if (f->mapa[rlin][rcol] == SIM_VAZIO) {
                    f->mapa[rlin][rcol] = SIM_ESPINHO;
                    f->espinhos[f->num_espinhos].linha = rlin;
                    f->espinhos[f->num_espinhos].coluna = rcol;
                    f->num_espinhos++;
                }
            }
            return;
        }
        return;
    }

    // Movimento
    if (tecla == TECLA_CIMA)      nova_lin--;
    else if (tecla == TECLA_BAIXO) nova_lin++;
    else if (tecla == TECLA_ESQUERDA) nova_col--;
    else if (tecla == TECLA_DIREITA) nova_col++;
    else return;

    if (nova_lin < 0 || nova_lin >= f->nlin ||
        nova_col < 0 || nova_col >= f->ncol) return;
    char destino = f->mapa[nova_lin][nova_col];

    // Colisoes e logica
    if (destino == SIM_PAREDE) return;
    if (f == &fase_vila && destino == SIM_NPC) return;

    // Vila: coleta chave
    if (f == &fase_vila && destino == SIM_CHAVE && !f->chave_pega) {
        f->chave_pega = true;
        printf("Voce coletou a chave da Vila!\n");
        PAUSA();
        f->mapa[f->porta.linha][f->porta.coluna] = SIM_PORTA_ABERTA;
        f->porta_aberta = true;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        return;
    }
    // Vila: porta aberta avanca
    if (f == &fase_vila && destino == SIM_PORTA_ABERTA) {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        estado = TELA_FASE1;
        return;
    }

    // Fase 1: coleta chave
    if (f == &fase1 && destino == SIM_CHAVE && !f->chave_pega) {
        f->chave_pega = true;
        printf("Voce coletou a chave!\n");
        PAUSA();
        f->mapa[f->porta.linha][f->porta.coluna] = SIM_PORTA_ABERTA;
        f->porta_aberta = true;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        return;
    }
    // Fase 1: porta aberta avanca
    if (f == &fase1 && destino == SIM_PORTA_ABERTA) {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        estado = TELA_FASE2;
        return;
    }

    // Fase 2: coleta chave
    if (f == &fase2 && destino == SIM_CHAVE && !f->chave_pega) {
        f->chave_pega = true;
        printf("Voce coletou a chave da Fase 2!\n");
        PAUSA();
        f->mapa[f->porta.linha][f->porta.coluna] = SIM_PORTA_ABERTA;
        f->porta_aberta = true;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        return;
    }
    // Fase 2: porta aberta avanca
    if (f == &fase2 && destino == SIM_PORTA_ABERTA) {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        estado = TELA_FASE3;
        return;
    }

    // Fase 3: coleta chave
    if (f == &fase3 && destino == SIM_CHAVE && !f->chave_pega) {
        f->chave_pega = true;
        printf("Voce coletou a chave da Fase 3!\n");
        PAUSA();
        f->mapa[f->porta.linha][f->porta.coluna] = SIM_PORTA_ABERTA;
        f->porta_aberta = true;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        return;
    }
    // Fase 3: porta aberta avanca vitoria
    if (f == &fase3 && destino == SIM_PORTA_ABERTA) {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
        f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
        estado = TELA_VITORIA;
        return;
    }

    // Botao (Fase 2 e 3)
    if ((f == &fase2 || f == &fase3) && destino == SIM_BOTAO) {
        printf("Botao pressionado! Novos espinhos aparecerao.\n");
        PAUSA();
        for (int k = 0; k < 2; k++) {
            int rlin = rand() % (f->nlin - 2) + 1;
            int rcol = rand() % (f->ncol - 2) + 1;
            if (f->mapa[rlin][rcol] == SIM_VAZIO) {
                f->mapa[rlin][rcol] = SIM_ESPINHO;
                f->espinhos[f->num_espinhos].linha = rlin;
                f->espinhos[f->num_espinhos].coluna = rcol;
                f->num_espinhos++;
            }
        }
        return;
    }

    // Teleporte (Fase 3)
    if (f == &fase3 && destino == SIM_TELEPORTE) {
        teleporta(f, nova_lin, nova_col);
        return;
    }

    // Espinho reinicia fase
    if (destino == SIM_ESPINHO) {
        printf("Voce caiu em um espinho! Reiniciando esta fase.\n");
        PAUSA();
        reinicia_fase(f, (f == &fase1) ? 1 : (f == &fase2) ? 2 : 3);
        return;
    }
    // Monstro reinicia fase
    if (destino == SIM_MONSTRO1 || destino == SIM_MONSTRO2) {
        printf("Um monstro te pegou! Reiniciando esta fase.\n");
        PAUSA();
        reinicia_fase(f, (f == &fase1) ? 1 : (f == &fase2) ? 2 : 3);
        return;
    }

    // Movimento valido
    f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
    f->jogador.linha = nova_lin; f->jogador.coluna = nova_col;
    f->mapa[nova_lin][nova_col] = SIM_JOGADOR;
}

void reinicia_fase(Fase *f, int fase_num) {
    f->tentativas++;
    if (f->tentativas >= 3) {
        printf("Voce falhou 3 vezes. Voltando ao menu principal.\n");
        PAUSA();
        estado = TELA_MENU;
        return;
    }
    carrega_mapas();
    if (fase_num == 1) estado = TELA_FASE1;
    if (fase_num == 2) estado = TELA_FASE2;
    if (fase_num == 3) estado = TELA_FASE3;
}

void atualiza_monstros1(Fase *f) {
    for (int i = 0; i < f->num_monstros1; i++) {
        int lin = f->monstros1[i].linha, col = f->monstros1[i].coluna;
        f->mapa[lin][col] = SIM_VAZIO;
        int dir = rand() % 4;
        int nlin = lin, ncol = col;
        if (dir == 0) nlin--;
        else if (dir == 1) ncol++;
        else if (dir == 2) nlin++;
        else ncol--;
        char dest = f->mapa[nlin][ncol];
        if (dest != SIM_PAREDE && dest != SIM_ESPINHO && dest != SIM_CHAVE &&
            dest != SIM_PORTA_FECHADA && dest != SIM_BOTAO &&
            dest != SIM_TELEPORTE && dest != SIM_MONSTRO1 &&
            dest != SIM_MONSTRO2) {
            f->monstros1[i].linha = nlin;
            f->monstros1[i].coluna = ncol;
        }
        lin = f->monstros1[i].linha; col = f->monstros1[i].coluna;
        if (lin == f->jogador.linha && col == f->jogador.coluna) {
            printf("Monstro nivel 1 te pegou! Reiniciando fase.\n");
            PAUSA();
            reinicia_fase(f, (f == &fase2) ? 2 : 3);
            return;
        }
        f->mapa[lin][col] = SIM_MONSTRO1;
    }
}

void atualiza_monstros2(Fase *f) {
    for (int i = 0; i < f->num_monstros2; i++) {
        int lin = f->monstros2[i].linha, col = f->monstros2[i].coluna;
        f->mapa[lin][col] = SIM_VAZIO;
        int plin = f->jogador.linha, pcol = f->jogador.coluna;
        int nlin = lin, ncol = col;
        if (lin < plin) nlin++;
        else if (lin > plin) nlin--;
        if (col < pcol) ncol++;
        else if (col > pcol) ncol--;
        char dest = f->mapa[nlin][ncol];
        if (dest != SIM_PAREDE && dest != SIM_ESPINHO && dest != SIM_CHAVE &&
            dest != SIM_PORTA_FECHADA && dest != SIM_BOTAO &&
            dest != SIM_TELEPORTE && dest != SIM_MONSTRO1 &&
            dest != SIM_MONSTRO2) {
            f->monstros2[i].linha = nlin;
            f->monstros2[i].coluna = ncol;
        }
        lin = f->monstros2[i].linha; col = f->monstros2[i].coluna;
        if (lin == f->jogador.linha && col == f->jogador.coluna) {
            printf("Monstro nivel 2 te pegou! Reiniciando fase.\n");
            PAUSA();
            reinicia_fase(f, 3);
            return;
        }
        f->mapa[lin][col] = SIM_MONSTRO2;
    }
}

void teleporta(Fase *f, int lin, int col) {
    if (lin == f->tele1.linha && col == f->tele1.coluna) {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = f->tele2.linha;
        f->jogador.coluna = f->tele2.coluna;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_JOGADOR;
    } else {
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_VAZIO;
        f->jogador.linha = f->tele1.linha;
        f->jogador.coluna = f->tele1.coluna;
        f->mapa[f->jogador.linha][f->jogador.coluna] = SIM_JOGADOR;
    }
}

void jogo_vila() {
    while (estado == TELA_VILA) {
        desenha_tela(&fase_vila);
        char tecla = getch();
        processa_input(&fase_vila, tecla);
    }
}

void jogo_fase1() {
    while (estado == TELA_FASE1) {
        desenha_tela(&fase1);
        char tecla = getch();
        processa_input(&fase1, tecla);
    }
}

void jogo_fase2() {
    while (estado == TELA_FASE2) {
        desenha_tela(&fase2);
        char tecla = getch();
        processa_input(&fase2, tecla);
        if (estado != TELA_FASE2) return;
        atualiza_monstros1(&fase2);
    }
}

void jogo_fase3() {
    while (estado == TELA_FASE3) {
        desenha_tela(&fase3);
        char tecla = getch();
        processa_input(&fase3, tecla);
        if (estado != TELA_FASE3) return;
        atualiza_monstros1(&fase3);
        if (estado != TELA_FASE3) return;
        atualiza_monstros2(&fase3);
    }
}
