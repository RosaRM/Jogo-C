#include "raylib.h"
#include <stdio.h>
#include <math.h>

// Definição das constantes
#define s_width 1029 
#define s_height 900
#define MAX_OBSTACULOS 10

// Enumeração para as telas do menu
typedef enum {
    MENUS = 0,
    P_screen,
    R_screen
} Screen;

// Struct pra representar um obstáculo
typedef struct {
    Vector2 position;
    Texture2D texture;
    bool isActive;
} Obstaculo;

// Funções para desenhar as texturas de cada tela
void DrawMScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit);
void DrawPScreen(Texture2D buttonExit, Texture2D Pista,Texture2D Background, Texture2D BG, Texture2D Sky, Texture2D BG2, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float *pos_pista_x, float *pos_pista_y, float bgSpeed, float bgSpeedX, int *estado_player, float *SpriteTimer, float scale);
void DrawRScreen(Texture2D buttonExit);
void DrawVelocimetro(float speed, float max);
void InitObstaculo(Texture2D obstacleTexture);
void SpawnObstacle(float pistaX, float pistaY, float pistaWidth, Texture2D obstacleTexture, float pistaScale, Texture2D Pista);
void UpdateAndDrawObstacles(float bgSpeed, float bgSpeedX, int estado_player);

Obstaculo obstaculos[MAX_OBSTACULOS];
Texture2D obstacleTexture;

int main() {
    // Inicialização da janela
    InitWindow(s_width, s_height, "AE86 Race");

    // Carregamento das texturas
    Texture2D Pista = LoadTexture("play-img/Pista - Copia (2).png");
    Texture2D Background= LoadTexture("play-img/BG.png");
    Texture2D BG = LoadTexture("play-img/montanhas1.png");
    Texture2D BG2 = LoadTexture("play-img/montanhas.png");
    Texture2D Sky = LoadTexture("play-img/ceu.png");
    Texture2D menuBG = LoadTexture("menu-img/MENU.png");
    Texture2D Jogador = LoadTexture("play-img/carro.png");
    Texture2D Jogador_D1 = LoadTexture("play-img/carro_direita1.png");
    Texture2D Jogador_D2 = LoadTexture("play-img/carro_direita2.png");
    Texture2D Jogador_D3 = LoadTexture("play-img/carro_direita3.png");
    Texture2D Jogador_E1 = LoadTexture("play-img/carro_esquerda1.png");
    Texture2D Jogador_E2 = LoadTexture("play-img/carro_esquerda2.png");
    Texture2D Jogador_E3 = LoadTexture("play-img/carro_esquerda3.png");
    Texture2D buttonExit = LoadTexture("menu-img/quit.png");
    Texture2D buttonPlay = LoadTexture("menu-img/play.png");
    Texture2D buttonOptions = LoadTexture("menu-img/play_hover.png");
    obstacleTexture = LoadTexture("play-img/Obstaculo1.png");

    // Variáveis de controle
    Screen screenAtual = MENUS;
    float bgSpeedX = 0.4; // Velocidade da curva
    float bgSpeed = 0.0; 
    float maxSpeed = 0.7; // Velocidade máxima
    const float initiAcel = 0.00001; // Aceleração inicial
    const float initiDesacel = 0.0001; // Desaceleração inicial
    float acel = initiAcel;
    float SpriteTimer = 0.0;
    int estado_player = 0;
    float pos_pista_x = s_width / 2;
    float pos_pista_y = s_height - 120;
    float pistaScale = 0.8f; // Fator de escala para a pista
    int pos_x_player = 465;
    int pos_y_player = 720;

    InitObstaculo(obstacleTexture);

    // Temporizador para geração de obstáculos
    float obstacleSpawnTimer = 0.0f;
    const float obstacleSpawnInterval = 2.0f; // Gera obstáculo a cada 2 segundos

    // Loop principal
    while (!WindowShouldClose()) {
        // Atualização do temporizador de obstáculos
        obstacleSpawnTimer += GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE); // Mude a cor de fundo conforme necessário

        switch (screenAtual) {
            case MENUS:
                DrawMScreen(menuBG, buttonPlay, buttonOptions, buttonExit);

                // Lógica para detectar cliques nos botões
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 650, buttonExit.width, buttonExit.height})) {
                        CloseWindow();
                    } else if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 100, buttonPlay.width, buttonPlay.height})) {
                        screenAtual = P_screen;
                    } else if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - 150, 250, buttonOptions.width, buttonOptions.height})) {
                        screenAtual = R_screen;
                    }
                }
                break;

            case P_screen:
                DrawPScreen(buttonExit, Pista, Background, BG,  Sky, BG2, Jogador, Jogador_D1, Jogador_D2, Jogador_E1, Jogador_E2, &pos_pista_x, &pos_pista_y, bgSpeed, bgSpeedX, &estado_player, &SpriteTimer, pistaScale);

                // Atualização da posição e velocidade da pista
                pos_pista_y += bgSpeed * 1.2;
                
                // Checa se 
                if (pos_x_player + Jogador.width -20 > (pos_pista_x + (Pista.width * pistaScale) / 2) -56  ){
                    if ( bgSpeed > (20 * 0.7)/195) {// Diminui ate 20Kmh
                    bgSpeed -= initiDesacel; // Usar a desaceleração inicial para desacelerar
                        if (pos_x_player + Jogador.width - 20> (pos_pista_x + (Pista.width * pistaScale) / 2)){
                        pos_pista_x += bgSpeedX;
                        }
                    }
                }

                if (pos_x_player +20 < (pos_pista_x - (Pista.width * pistaScale) / 2) +56 ){
                    if ( bgSpeed > (20 * 0.7)/195) {// Diminui ate 20Kmh
                    bgSpeed -= initiDesacel; // Usar a desaceleração inicial para desacelerar
                        if (pos_x_player +20< (pos_pista_x - (Pista.width * pistaScale) / 2)  ){
                        pos_pista_x -= bgSpeedX;
                        }
                    }
                }

                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                    // Aumentar a velocidade gradualmente até a velocidade máxima
                    if (bgSpeed < maxSpeed) {
                        bgSpeed += acel;
                        if (bgSpeed > 0.005) {
                            acel += 0.00000001;
                        }
                        if (bgSpeed > maxSpeed) {
                            bgSpeed = maxSpeed;
                        }
                    }
                } else {
                    acel = initiAcel; // Redefinir a aceleração para o valor inicial
                    // Reduzir a velocidade gradualmente quando a tecla não está pressionada
                    if (bgSpeed > 0) {
                        bgSpeed -= acel; // A desaceleração 
                        if (bgSpeed <= 0) {
                            bgSpeed = 0;
                        }
                    }
                }
                // Desacelerar ao pressionar a tecla S
                if (IsKeyDown(KEY_S) || (IsKeyDown(KEY_DOWN) && bgSpeed > 0)) {
                    bgSpeed -= initiDesacel; // Usar a aceleração inicial para desacelerar
                    if (bgSpeed <= 0) {
                        bgSpeed = 0;
                    }
                }


                // Movimenta a pista e modifica variavel para mudar Sprite do Carro
                if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                    SpriteTimer += bgSpeedX;
                    // Reduzir a velocidade gradualmente quando a tecla não está pressionada
                    if (bgSpeed > 0.4) {
                        pos_pista_x -= bgSpeedX;
                        bgSpeed -= acel * 1.2; // A desaceleração é mais lenta
                        if (bgSpeed <= 0) {
                            bgSpeed = 0;
                        }
                    } else {
                        pos_pista_x -= bgSpeed;
                    }
                    if (SpriteTimer > 50) {
                        estado_player = 2;
                        if (SpriteTimer > 100) {
                        estado_player = 3;
                        SpriteTimer = 101;
                        }
                    } else {
                        estado_player = 1;
                    }
                } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                    SpriteTimer -= bgSpeedX;
                    // Reduzir a velocidade gradualmente quando a tecla não está pressionada
                    if (bgSpeed > 0.4) {
                        pos_pista_x += bgSpeedX;
                        bgSpeed -= acel * 1.2; // A desaceleração é mais lenta
                        if (bgSpeed <= 0) {
                            bgSpeed = 0;
                        }
                    } else {
                        pos_pista_x += bgSpeed;
                    }
                    if (SpriteTimer < -50) {
                        estado_player = 5;
                        if (SpriteTimer < -100) {
                        SpriteTimer = -101;
                        estado_player = 6;
                        }
                    } else {
                        estado_player = 4;
                    }
                } else {
                    SpriteTimer = 0;
                    estado_player = 0;
                }

                // Desenha a sprite do jogador de acordo com o estado para animação de curva
                switch (estado_player) {
                    case 0:
                        DrawTexture(Jogador, pos_x_player, pos_y_player, WHITE); // Sprite padrão
                        break;
                    case 1:
                        DrawTexture(Jogador_D1, pos_x_player, pos_y_player, WHITE); // Sprite de curva direita 1
                        break;
                    case 2:
                        DrawTexture(Jogador_D2, pos_x_player, pos_y_player, WHITE); // Sprite de curva direita 1
                        break;
                    case 3:
                        DrawTexture(Jogador_D3, pos_x_player, pos_y_player, WHITE); // Sprite de curva direita 2
                        break;
                    case 4:
                        DrawTexture(Jogador_E1, pos_x_player, pos_y_player, WHITE); // Sprite de curva esquerda 1
                        break;
                    case 5:
                        DrawTexture(Jogador_E2, pos_x_player, pos_y_player, WHITE); // Sprite de curva esquerda 1
                        break;
                    case 6:
                        DrawTexture(Jogador_E3, pos_x_player, pos_y_player, WHITE); // Sprite de curva esquerda 2
                        break;
                }

                // Gera obstáculos periodicamente
                if (obstacleSpawnTimer >= obstacleSpawnInterval) {
                    SpawnObstacle(pos_pista_x , pos_pista_y, Pista.width * pistaScale, obstacleTexture, pistaScale, Pista);
                    obstacleSpawnTimer = 0.0f; // Reinicia o temporizador
                }

                // Atualiza e desenha obstáculos
                UpdateAndDrawObstacles(bgSpeed, bgSpeedX, estado_player);

                // Lógica para detectar clique no botão de saída na tela de ranking
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }

                // Atualização do velocímetro
                DrawVelocimetro(bgSpeed, maxSpeed);

                break;

            case R_screen:
                DrawRScreen(buttonExit);
                // Lógica para detectar clique no botão de saída na tela de ranking
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

    // Descarregamento das texturas
    UnloadTexture(Pista);
    UnloadTexture(Background);
    UnloadTexture(BG);
    UnloadTexture(BG2);   
    UnloadTexture(Sky);
    UnloadTexture(menuBG);
    UnloadTexture(Jogador);
    UnloadTexture(Jogador_D1);
    UnloadTexture(Jogador_D2);
    UnloadTexture(Jogador_E1);
    UnloadTexture(Jogador_E2);
    UnloadTexture(buttonExit);
    UnloadTexture(buttonPlay);
    UnloadTexture(buttonOptions);
    UnloadTexture(obstacleTexture);

    // Fechamento da janela
    CloseWindow();

    return 0;
}

// Função para inicializar os obstáculos
void InitObstaculo(Texture2D obstacleTexture) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        // Inicializa a posição do obstáculo como (0, 0)
        obstaculos[i].position = (Vector2){0, 0};
        // Define a textura do obstáculo como a textura carregada
        obstaculos[i].texture = obstacleTexture;
        // Define o obstáculo como inativo inicialmente
        obstaculos[i].isActive = false;
    }
}

// Função para gerar um obstáculo em uma posição específica da pista
void SpawnObstacle(float pistaX, float pistaY, float pistaWidth, Texture2D obstacleTexture, float pistaScale, Texture2D Pista) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        // Verifica se há algum obstáculo inativo para ser ativado
        if (!obstaculos[i].isActive) {
            // Define a posição do obstáculo na pista, na parte superior visível
            obstaculos[i].position = (Vector2){ pistaX + GetRandomValue(56, pistaWidth - obstacleTexture.width - 56), pistaY - ((Pista.height * pistaScale)*(5*i))};
            // Ativa o obstáculo
            obstaculos[i].isActive = true;
            break; // Sai do loop após ativar um obstáculo
        }
    }
}

void UpdateAndDrawObstacles(float bgSpeed, float bgSpeedX, int estado_player) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].isActive) {
            // Atualiza a posição vertical do obstáculo com base na velocidade da pista
            obstaculos[i].position.y += bgSpeed;
            
            // Atualiza a posição horizontal do obstáculo com base no movimento lateral do jogador
            if (estado_player == 1) { // Move para a direita
                obstaculos[i].position.x -= bgSpeedX;
            } else if (estado_player == 3) { // Move para a esquerda
                obstaculos[i].position.x += bgSpeedX;
            }
            
            // Verifica se o obstáculo saiu da tela
            if (obstaculos[i].position.y > s_height) {
                obstaculos[i].isActive = false;
            } else {
                // Desenha o obstáculo na tela
                DrawTexture(obstaculos[i].texture, obstaculos[i].position.x, obstaculos[i].position.y, WHITE);
            }
        }
    }
}




void DrawMScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit) {
    ClearBackground(BLACK); 
    DrawTexture(menuBG, 265, 50, WHITE);  
    DrawTexture(buttonPlay, s_width / 2 - 150, 100, WHITE);
    DrawTexture(buttonOptions, s_width / 2 - 150, 250, WHITE);
    DrawTexture(buttonExit, s_width / 2 - 150, 650, WHITE);
}

void DrawPScreen(Texture2D buttonExit, Texture2D Pista,Texture2D Background, Texture2D BG,Texture2D Sky, Texture2D BG2, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float *pos_pista_x, float *pos_pista_y, float bgSpeed, float bgSpeedX, int *estado_player, float *SpriteTimer, float scale) {

    int pos_BG_y = 300;

    ClearBackground(DARKPURPLE);
    float currentY = *pos_pista_y; // Define onde começa a desenhar a pista
    scale = 0.8;

    // Desenhar as camadas da pista para criar a perspectiva
    while (currentY > pos_BG_y - Pista.height * scale) {
        // Calcular a posição x para centralizar a pista
        float posX = *pos_pista_x - (Pista.width * scale) / 2;
        
        // Desenha a textura da pista com a escala
        DrawTextureEx(Pista, (Vector2){posX, currentY}, 0.0f, scale, WHITE);

        // Atualiza a posição vertical para a próxima camada
        currentY -= Pista.height * scale;
    }

    // Desenha o fundo depois
    DrawTextureEx(Sky , (Vector2){0, -300}, 0, 0.9, WHITE);
    DrawTextureEx(BG2 , (Vector2){-100, 75}, 0, 0.8, WHITE);    
    DrawTextureEx(BG2 , (Vector2){600, 115}, 0, 0.8, WHITE);
    DrawTextureEx(BG , (Vector2){0, 0}, 0, 0.8, WHITE);
    DrawTextureEx(Background , (Vector2){0, 300}, 0, 1, WHITE);

    // Atualiza e desenha obstáculos
    UpdateAndDrawObstacles(bgSpeed, bgSpeedX, *estado_player);

    // Desenha o gotão de saída
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
    
}

void DrawRScreen(Texture2D buttonExit) {
    ClearBackground(BLUE);
    DrawText("Ranking Screen", s_width / 2 - MeasureText("Ranking Screen", 20) / 2, s_height / 2 - 10, 20, WHITE);
    DrawTexture(buttonExit, s_width / 2 - buttonExit.width / 2, 50, WHITE);
}

void DrawVelocimetro(float speed, float maxSpeed) {
    // Convertendo a velocidade de pixels por frame para Km/h
    float Kmh = (speed * 195) / maxSpeed;

    // Limitando o valor máximo do velocímetro a 195 km/h
    if (Kmh > 195)
        Kmh = 195;

    // Medindo as dimensões do texto
    float textWidth = MeasureText(TextFormat("%.0f Km/h", Kmh), 25);

    // Definindo a posição inicial do velocímetro (canto superior esquerdo)
    int posX = 800;
    int posY = 665;
    int centerPosX = posX + 150;
    int centerPosY = posY + 150;

    // Definindo o ângulo do ponteiro (sentido anti-horário, 0° é à direita)
    float angle = 220 - Kmh * 1.128; 

    // Normalizando o ângulo para o intervalo [0, 360]
    angle = fmod(angle, 360);
    if (angle < 0)
        angle += 360;


    // Desenhando o círculo do velocímetro (centro)
    DrawCircle(centerPosX, centerPosY, 70, Fade(BLACK, 0.9));

    // Desenhando o contorno cinza escuro ao redor do velocímetro
    DrawRing((Vector2){ centerPosX, centerPosY }, 72, 67, 0, 360, 20, DARKGRAY);

    // Desenhando a linha do velocímetro
    DrawLine(centerPosX, centerPosY, centerPosX + 50 * cos(angle * DEG2RAD), centerPosY - 50 * sin(angle * DEG2RAD), RED);

    // Posição da bola vermelha (indicador da velocidade)
    float indicatorX = centerPosX + 50 * cos(angle * DEG2RAD);
    float indicatorY = centerPosY - 50 * sin(angle * DEG2RAD);

    // Desenhando o indicador da velocidade atual (bola vermelha)
    DrawCircle(indicatorX, indicatorY, 5, RED);

    // Desenhando o texto com a velocidade
    DrawText(TextFormat("%.0f Km/h", Kmh), centerPosX - textWidth / 2 +20, centerPosY + 40, 15, GREEN);
}

