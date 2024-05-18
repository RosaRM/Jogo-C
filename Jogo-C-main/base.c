#include "raylib.h"
#include <stdio.h>

// Definição das constantes
#define s_width 1029 
#define s_height 900

// Enumeração para as telas do menu
typedef enum {
    MENUS = 0,
    play_screen,
    Rank_screen
} Screen;

int main() {
    // Inicialização da janela
    InitWindow(s_width, s_height, "AE86 Race");
    
    // Carregamento da imagem do fundo 
    Texture2D Pista = LoadTexture("play-img/Pista.png");
    Texture2D BG = LoadTexture("play-img/BG.png");
    Texture2D menuBG = LoadTexture("menu-img/MENU.png");
    // Carregamento da sprite do jogador
    Texture2D Jogador = LoadTexture("play-img/carro.png");
    // Sprites de curva pra  direita
    Texture2D Jogador_D1 = LoadTexture("play-img/carro_direita1.png");
    Texture2D Jogador_D2 = LoadTexture("play-img/carro_direita2.png");
    // Sprites de curva pra  esquerda
    Texture2D Jogador_E1 = LoadTexture("play-img/carro_esquerda1.png");
    Texture2D Jogador_E2 = LoadTexture("play-img/carro_esquerda2.png");
    // carregamento das imagens dos botões
    Texture2D buttonExit = LoadTexture("menu-img/quit.png");
    Texture2D buttonPlay = LoadTexture("menu-img/play.png");
    Texture2D buttonOptions = LoadTexture("menu-img/play_hover.png");


    // Posição dos botões largura
    int Button_x_pos = s_width / 2 - 150;
    // Posição do jogador
    int pos_x_player = 465;
    int pos_y_player = 555;
    // Posição inicial do fundo
    float pos_x_bg = 50;
    int pos_y_bg = 555;    
    // Definição da posição Y inicial da pista
    int pos_y_pista = s_height - Pista.height;

    // Velocidade de movimento horizontal do fundo
    float bgSpeed = 0.5;

    // Definição da variável para controle da tela atual
    Screen screenAtual = MENUS;

    // Variável para controlar o estado da sprite do jogador
    int estado_player = 0; // 0 para sprite padrão, 1 para sprite de curva direita 1, 2 para sprite de curva direita 2, 3 para sprite de curva esquerda 1, 4 para sprite de curva esquerda 2

    // Variável para controlar a mudança de sprite
    float SpriteTimer = 0.0;

    // Loop principal
    while (!WindowShouldClose()) {
        // lógica para determinar qual tela desenhar
        switch (screenAtual) {
            case MENUS:
                // Desenhar a tela do menu
                BeginDrawing();
                ClearBackground(BLACK); 
                //desenha o fundo do menu
                DrawTexture(menuBG, 265, 50, WHITE);  
                //desenhar botões     
                DrawTexture(buttonPlay, Button_x_pos , 100, WHITE);
                DrawTexture(buttonOptions, Button_x_pos, 250, WHITE);
                DrawTexture(buttonExit, Button_x_pos, 650, WHITE);
                EndDrawing();

                // Lógica para detectar cliques nos botões
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    // verificar se o clique ocorreu em algum dos botões
                    if (CheckCollisionPointRec(mousePoint, 
                        (Rectangle){s_width / 2 - 150, 650, buttonExit.width, buttonExit.height})) {
                        // Clicou no botão Exit
                        CloseWindow();
                    } else if (CheckCollisionPointRec(mousePoint, 
                        (Rectangle){s_width / 2 - 150, 100, buttonPlay.width, buttonPlay.height})) {
                        // Clicou no botão Play
                        screenAtual = play_screen;
                    } else if (CheckCollisionPointRec(mousePoint, 
                        (Rectangle){s_width / 2 - 150, 250, buttonOptions.width, buttonOptions.height})) {
                        // Clicou no botão Options
                        screenAtual = Rank_screen;
                    }
                }
                break;
            
            case play_screen:
            // Lógica para o movimento horixontal do fundo e mudar a sprite do jogado
             if (IsKeyDown(KEY_D)) {
                    pos_x_bg -= bgSpeed; // Move o fundo para a esquerda
                    SpriteTimer += bgSpeed; // adiciona pra mudar a sprite
                // ve se é hora de mudar a sprite
                if (SpriteTimer > 125) {
                        estado_player = 2; // Altera para a próxima sprite de curva direita
                 
                } else {                   
                    estado_player = 1; // Altera o estado da sprite do jogador para curva direita 1
                }
                } else if (IsKeyDown(KEY_A)) {
                    pos_x_bg += bgSpeed; // move o fundo para a esquerda
                    SpriteTimer += bgSpeed; // adiciona pra mudar a sprite
                // Verifica se é hora de mudar a sprite
                if (SpriteTimer > 125) {
                        estado_player = 4; // Altera para a próxima sprite de curva direita
                 
                } else {                   
                    estado_player = 3; // Altera o estado da sprite do jogador para curva direita 1
                }
                } else {
                    SpriteTimer = 0; // zera pra modificar novamente
                    estado_player = 0; // retorna à sprite padrão quando não há movimento horizontal
                }

                // Desenhar a tela de jogo
                BeginDrawing();
                ClearBackground(BLANK);
                DrawTexture(BG, 0, 360, WHITE); // Desenha a imagem de fundo
                // Desenhar os tiles da pista que se estendem para o horizonte
                int current_y = pos_y_pista;
                while (current_y >= -Pista.height) {
                    DrawTexture(Pista, pos_x_bg, current_y, WHITE);
                    current_y -= Pista.height;
                }
            

                //desenha a sprite do jogador de acordo com o estado
                switch (estado_player) {
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

                DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE); // Desenha o botão de volta
                EndDrawing();

                // Lógica para voltar ao menu
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, 
                        (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }
                break;
            
            case Rank_screen:
                // Desenhar a tela de opções
                BeginDrawing();
                ClearBackground(GREEN);
                DrawText("OPÇÕES / RANKING", 100, 250, 50, WHITE);
                DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE); // Desenha o botão de volta
                EndDrawing();

                // Lógica para voltar ao menu
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, 
                        (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }
                break;
        }
    }
    
    // Descarrega as texturas 
    UnloadTexture(Pista);
    UnloadTexture(menuBG); 
    UnloadTexture(BG); 
    UnloadTexture(buttonExit);
    UnloadTexture(buttonPlay);
    UnloadTexture(buttonOptions);
    UnloadTexture(Jogador);
    UnloadTexture(Jogador_D1);
    UnloadTexture(Jogador_D2);
    UnloadTexture(Jogador_E1);
    UnloadTexture(Jogador_E2);

    //Fecha a janela
    CloseWindow();
}