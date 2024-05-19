#include "raylib.h"
#include <stdio.h>

// Definição das constantes
#define s_width 1029 
#define s_heigh 900


// Enumeração para as telas do menu
typedef enum {
    MENUS = 0,
    PLAY_SCREEN,
    RANK_SCREEN
} Screen;

// Declarar funções anters do loop principal para funções que possam chamar umas as  outrsas

//Funçõesp ara desnhar as texturas de cada tela
void DrawMenuScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit);                             // O simbolo * são para definirmos as variaveis como ponteiros, = para podermos modifica las dentro das funções
void DrawPlayScreen(Texture2D buttonExit, Texture2D Pista, Texture2D BG, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2,float *pos_pista_x, float *pos_pista_y, float bgSpeed, int *estado_player, float *SpriteTimer);
void DrawRankScreen(Texture2D buttonExit);

int main() {
    // Inicialização da janela
    InitWindow(s_width, s_heigh, "AE86 Race");

    // Carregamento das texturas
    Texture2D Pista = LoadTexture("play-img/Pista.png");
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
    Screen screenAtual = MENUS;
    float bgSpeed = 0.5;
    float SpriteTimer = 0.0;
    int estado_player = 0;
    float pos_pista_x = s_width / 2;
    float pos_pista_y = s_heigh - 120;

    // Loop principal
    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            pos_pista_y += bgSpeed; // Move a pista para baixo
        }
        //APENAS PARA TESTES 
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            pos_pista_y -= bgSpeed; // Move a pista para baixo
        }

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
                        screenAtual = PLAY_SCREEN;
                    } else if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 250, buttonOptions.width, buttonOptions.height})) {
                        screenAtual = RANK_SCREEN;
                    }
                }
                break;

            case PLAY_SCREEN:
                DrawPlayScreen(buttonExit, Pista, BG, Jogador, Jogador_D1, Jogador_D2, Jogador_E1, Jogador_E2, &pos_pista_x, &pos_pista_y, bgSpeed, &estado_player, &SpriteTimer);

                // Lógica para voltar ao menu
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }
                break;

            case RANK_SCREEN:
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
    UnloadTexture(Pista);
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

// Chama variavel de desenhar a tela de Menu
void DrawMenuScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit) {
    ClearBackground(BLACK); 
    //Desneha elementos da tela
    DrawTexture(menuBG, 265, 50, WHITE);  
    DrawTexture(buttonPlay, s_width / 2 - 150, 100, WHITE);
    DrawTexture(buttonOptions, s_width / 2 - 150, 250, WHITE);
    DrawTexture(buttonExit, s_width / 2 - 150, 650, WHITE);
}

//Chama variavel de desenhar a tela de jogo
void DrawPlayScreen(Texture2D buttonExit, Texture2D Pista, Texture2D BG, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float *pos_pista_x, float *pos_pista_y, float bgSpeed, int *estado_player, float *SpriteTimer) {
    int pos_x_player = 465;
    int pos_y_player = 800;
    int pos_BG_y = 600;

    // Movimenta A pista e modifica variavel para mudar Sprite do Carro
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        *pos_pista_x -= bgSpeed;
        *SpriteTimer += bgSpeed;

        if (*SpriteTimer > 100) {
            *estado_player = 2;
            *SpriteTimer = 100;
        } else {
            *estado_player = 1;
        }
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        *pos_pista_x += bgSpeed;
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
    float currentY = *pos_pista_y; // Define onde começa a desenhar a pista
    float Scale = 1.0f; // Escala inicial
    const float Mut_scale = 0.5f;
    const float minScale = 0.0009765625;

    

    // Desenhar as camadas da pista para criar a perspectiva
    while (currentY > pos_BG_y + 63) {
        // Calcular a posiçao x para centralizar a pista
        float posX = *pos_pista_x - (Pista.width * Scale) / 2;

        // Desenha a textura da pista
        DrawTextureEx(Pista, (Vector2){posX, currentY}, 0, Scale, WHITE);

        // Atualiza a posição vertical para a próxima camada
        currentY -= Pista.height * Scale * Mut_scale;

        // Diminui a escala
        Scale *= Mut_scale;
        if (Scale < minScale) Scale = minScale; // Garante que a escala não fique menor que o mínimo e desenhe a sprite infinitamente
    }

        // Desenha o fundo deposi
    DrawTexture(BG, 0, pos_BG_y, WHITE);

        // Desenha a sprite do jogador de acordo com o estado apara animação de curva
    switch (*estado_player) {
        case 0:
            DrawTexture(Jogador, pos_x_player, pos_y_player, WHITE); // Sprite padrão
            break;
        case 1:
            DrawTexture(Jogador_D1, pos_x_player, pos_y_player, WHITE); // Sprite de curva direita 1
            break;
        case 2:
            DrawTexture(Jogador_D2, pos_x_player, pos_y_player, WHITE); // Sprite de curva direita 2
            break;
        case 3:
            DrawTexture(Jogador_E1, pos_x_player, pos_y_player, WHITE); // Sprite de curva esquerda 1
            break;
        case 4:
            DrawTexture(Jogador_E2, pos_x_player, pos_y_player, WHITE); // Sprite de curva esquerda 2
            break;
    }

    // Desenha o botão de saída
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
}

//Chama variavel de desenhar a tela de ranking
void DrawRankScreen(Texture2D buttonExit) {
    ClearBackground(BLUE);
    DrawText("Ranking Screen", s_width / 2 - MeasureText("Ranking Screen", 20) / 2, s_heigh / 2 - 10, 20, WHITE);
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
}
