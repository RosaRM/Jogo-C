#include "raylib.h"
#include <stdio.h>

// Definição das constantes
#define s_width 1029 
#define s_height 900

// Enumeração para as telas do menu
typedef enum {
    MENUS = 0,
    P_SCREEN,
    R_SCREEN
} Screen;

typedef struct {
    Texture2D texture;  
    float posX;         
    float posY;         
    float scale;        
} Pista;

// Declarar funções antes do loop principal para funções que possam chamar umas às outras

// Funções para desenhar as texturas de cada tela
void DrawMenuScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit);
void DrawPlayScreen(Texture2D buttonExit, Pista *pista, Texture2D BG, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float bgSpeed, int *estado_player, float *SpriteTimer);
void DrawRankScreen(Texture2D buttonExit);

int main() {
    // Inicialização da janela
    InitWindow(s_width, s_height, "AE86 Race");

    // Definições das variáveis da struct Pista
    Pista pista;
    pista.texture = LoadTexture("play-img/Pista.png");
    pista.posX = s_width / 2;
    pista.posY = s_height - 112;
    pista.scale = 1.0f;

    // Carregamento das texturas
    Texture2D BG = LoadTexture("play-img/BG.png");
    Texture2D menuBG = LoadTexture("menu-img/MENU.png");
    Texture2D Jogador = LoadTexture("play-img/carro.png");
    Texture2D Jogador_D1 = LoadTexture("play-img/carro_direita1.png");
    Texture2D Jogador_D2 = LoadTexture("play-img/carro_direita2.png");
    Texture2D Jogador_E1 = LoadTexture("play-img/carro_esquerda1.png");
    Texture2D Jogador_E2 = LoadTexture("play-img/carro_esquerda2.png");
    Texture2D buttonExit = LoadTexture("menu-img/quit.png");
    Texture2D buttonPlay = LoadTexture("menu-img/play.png");
    Texture2D buttonOptions = LoadTexture("menu-img/play_hover.png");

    // Variáveis de controle
    Screen screenAtual = P_SCREEN;
    float bgSpeed = 0.5;
    float SpriteTimer = 0.0;
    int estado_player = 0;

    // Loop principal
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE); // Mude a cor de fundo conforme necessário

        switch (screenAtual) {
            case MENUS:
                DrawMenuScreen(menuBG, buttonPlay, buttonOptions, buttonExit);

                // Lógica para detectar cliques nos botões
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 650, buttonExit.width, buttonExit.height})) {
                        CloseWindow();
                    } else if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 100, buttonPlay.width, buttonPlay.height})) {
                        screenAtual = P_SCREEN;
                    } else if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 250, buttonOptions.width, buttonOptions.height})) {
                        screenAtual = R_SCREEN;
                    }
                }
                break;

            case P_SCREEN:
                DrawPlayScreen(buttonExit, &pista, BG, Jogador, Jogador_D1, Jogador_D2, Jogador_E1, Jogador_E2, bgSpeed, &estado_player, &SpriteTimer);
                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                    pista.posY += bgSpeed; // Move a pista para baixo
                }
                //APENAS PARA TESTES
                if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                    pista.posY -= bgSpeed; // Move a pista para cima
                }
                // Lógica para voltar ao menu
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }
                break;

            case R_SCREEN:
                DrawRankScreen(buttonExit);

                // Lógica para voltar ao menu
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }
                break;
        }

        EndDrawing();
    }

    // Descarregamento das texturas e fechamento da janela
    UnloadTexture(pista.texture);
    UnloadTexture(BG);
    UnloadTexture(menuBG);
    UnloadTexture(Jogador);
    UnloadTexture(Jogador_D1);
    UnloadTexture(Jogador_D2);
    UnloadTexture(Jogador_E1);
    UnloadTexture(Jogador_E2);
    UnloadTexture(buttonExit);
    UnloadTexture(buttonPlay);
    UnloadTexture(buttonOptions);

    CloseWindow();
    return 0;
}

// Função para desenhar a tela de Menu
void DrawMenuScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit) {
    ClearBackground(BLACK); 
    // Desenha elementos da tela
    DrawTexture(menuBG, 265, 50, WHITE);  
    DrawTexture(buttonPlay, s_width / 2 - 150, 100, WHITE);
    DrawTexture(buttonOptions, s_width / 2 - 150, 250, WHITE);
    DrawTexture(buttonExit, s_width / 2 - 150, 650, WHITE);
}

// Função para desenhar a tela de jogo
void DrawPlayScreen(Texture2D buttonExit, Pista *pista, Texture2D BG, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float bgSpeed, int *estado_player, float *SpriteTimer) {
    int pos_player_x = pista->texture.width / 2;
    int pos_player_y = 800;
    int pos_BG_y = 600;

    // Movimenta a pista e modifica variáveis para mudar a Sprite do Carro
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        pista->posX -= bgSpeed;
        *SpriteTimer += bgSpeed;

        if (*SpriteTimer > 100) {
            *estado_player = 2;
            *SpriteTimer = 100;
        } else {
            *estado_player = 1;
        }
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        pista->posX += bgSpeed;
        *SpriteTimer -= bgSpeed;

        if (*SpriteTimer < -100) {
            *estado_player = 4;
            *SpriteTimer = -100;
        } else {
            *estado_player = 3;
        }
    } else {
        *SpriteTimer = 0;
        *estado_player = 0;
    }


    ClearBackground(DARKPURPLE);
    float currentY = pista->posY; // Define onde começa a desenhar a pista
    float Scale = pista->scale; // Escala inicial
    const float Mut_scale = 0.5f;
    const float minScale = 0.0009765625;


    // Desenhar as camadas da pista para criar a perspectiva
    while (currentY > pos_BG_y + 63) {
        // Calcular a posição x para centralizar a pista
        float posX = pista->posX - (pista->texture.width * Scale) / 2;

        // Desenha a textura da pista
        DrawTextureEx(pista->texture, (Vector2){posX, currentY}, 0, Scale, WHITE);

        // Atualiza a posição vertical para a próxima camada
        currentY -= pista->texture.height * Scale * Mut_scale;

        // Diminui a escala
        Scale *= Mut_scale;
        if (Scale < minScale) Scale = minScale; // Garante que a escala não fique menor que o mínimo e desenhe a sprite infinitamente
    }

    // Desenha o fundo depois
    DrawTexture(BG, 0, pos_BG_y, WHITE);

    // Desenha a sprite do jogador de acordo com o estado para animação de curva
    switch (*estado_player) {
        case 0:
            DrawTexture(Jogador, pos_player_x, pos_player_y, WHITE); // Sprite padrão
            break;
        case 1:
            DrawTexture(Jogador_D1, pos_player_x, pos_player_y, WHITE); // Sprite de curva direita 1
            break;
        case 2:
            DrawTexture(Jogador_D2, pos_player_x, pos_player_y, WHITE); // Sprite de curva direita 2
            break;
        case 3:
            DrawTexture(Jogador_E1, pos_player_x, pos_player_y, WHITE); // Sprite de curva esquerda 1
            break;
        case 4:
            DrawTexture(Jogador_E2, pos_player_x, pos_player_y, WHITE); // Sprite de curva esquerda 2
            break;
    }

    // Desenha o botão de saída
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
}

// Função para desenhar a tela de ranking
void DrawRankScreen(Texture2D buttonExit) {
    ClearBackground(BLUE);
    DrawText("Ranking Screen", s_width / 2 - MeasureText("Ranking Screen", 20) / 2, s_height / 2 - 10, 20, WHITE);
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
}
