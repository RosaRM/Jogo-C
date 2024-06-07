#include "raylib.h"
#include <stdio.h>
#include <math.h>

// Definição das constantes
#define s_width 1024
#define s_height 900
#define MAX_OBSTACULOS 500

// Enumeração para as telas do menu
typedef enum {
    MENUS = 0,
    P_screen,
    R_screen
} Screen;

// Struct pra representar um obstaculo
typedef struct {
    Vector2 position;
    Texture2D texture;
    bool Ativo;
} Obstaculo;

// Funçoes para desenhar as texturas de cada tela
void DrawMScreen(Texture2D menuBG, Texture2D buttonPlay, Texture2D buttonOptions, Texture2D buttonExit);                    // OS * servem para definir ponteiros para podermos modificalos dentro da função
void DrawPScreen(Texture2D buttonExit, Texture2D Pista,Texture2D Background, Texture2D BG, Texture2D Sky, Texture2D BG2, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float *pos_pista_x, float *pos_pista_y, float *bgSpeed, float bgSpeedX, int *estado_player, float *SpriteTimer, float scale, int pos_x_player, Rectangle AreaJogador, Texture2D Grama);
void DrawRScreen(Texture2D buttonExit);
void DrawVelocimetro(float speed, float max);
void InitObstaculo(Texture2D obstacleTexture);
void SpawnObstaculo(float pistaX, float pistaY, float pistaWidth, Texture2D obstacleTexture, float pistaScale, Texture2D Pista);
void UpdateEDrawObstaculo(float *bgSpeed, float bgSpeedX, int estado_player, float posX , Texture2D Pista,  Texture2D Jogador, int pos_x_player, Rectangle AreaJogador);

Obstaculo obstaculos[MAX_OBSTACULOS];
Texture2D obstacleTexture;

int main() {
    // Inicialização da janela
    InitWindow(s_width, s_height, "AE86 Race");

    // Inicialização do áudio
    InitAudioDevice();

    // Carregamento da música
    Music music = LoadMusicStream("Audios/Musica.wav");
    PlayMusicStream(music);

    // Carregamento das texturas
    Texture2D Grama = LoadTexture("play-img/grama2.png");
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

    // Variaveis de controle
    Screen screenAtual = MENUS;
    float bgSpeed = 0.0;
    float maxSpeed = 0.7; // Velocidade maxima
    const float initiAcel = 0.00003; // Aceleração inicial
    const float initiDesacel = 0.0001; // Desaceleração inicial
    float acel = initiAcel;
    float SpriteTimer = 0.0;
    int estado_player = 0;    
    float pistaScale = 0.8f; // Fator de escala para a pista
    float pos_pista_x = s_width / 2  - (Pista.width * pistaScale) / 2;
    float pos_pista_y = s_height - 120;
    int pos_x_player = 465;
    int pos_y_player = 720;

    Rectangle AreaJogador = { pos_x_player, 720, Jogador.width, Jogador.height };

    InitObstaculo(obstacleTexture);

    // Temporizador para geração de obstaculos
    float obstacleSpawnTimer = 0.0f;
    float obstacleSpawnInterval = 5.0f; // Gera obstaculo a cada 5 segundos

    // Loop principal
    while (!WindowShouldClose()) {

        UpdateMusicStream(music);

        float bgSpeedX = bgSpeed; // Velocidade da curva
        // Atualização do temporizador de obstaculos
        obstacleSpawnTimer += GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE); // Mude a cor de fundo conforme necessario

        switch (screenAtual) {
            case MENUS:
                DrawMScreen(menuBG, buttonPlay, buttonOptions, buttonExit);

                // Logica para detectar cliques nos botoes
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
                DrawPScreen(buttonExit, Pista, Background, BG,  Sky, BG2, Jogador, Jogador_D1, Jogador_D2, Jogador_E1, Jogador_E2, &pos_pista_x, &pos_pista_y, &bgSpeed, bgSpeedX, &estado_player, &SpriteTimer, pistaScale, pos_x_player, AreaJogador, Grama);

                // Atualização da posição e velocidade da pista
                pos_pista_y += bgSpeed * 1.2;
                
                // Checa se jogador colide com lado direito da pista
                if (pos_x_player + Jogador.width > (pos_pista_x + (Pista.width * pistaScale) ) -56  ){
                    if ( bgSpeed > (20 * maxSpeed)/195) {// Diminui ate 20Kmh
                    bgSpeed -= initiDesacel; // Usar a desaceleração inicial para desacelerar
                    }
                    if (pos_x_player + Jogador.width  - 20> (pos_pista_x + (Pista.width * pistaScale) )){
                        pos_pista_x += bgSpeedX;
                    }
                }
                // Checa se jogador colide com lado esquerdo da pista
                if (pos_x_player +20 < (pos_pista_x) +56 ){
                    if ( bgSpeed > (20 * maxSpeed)/195) {// Diminui ate 20Kmh
                    bgSpeed -= initiDesacel; // Usar a desaceleração inicial para desacelerar
                    }
                    if (pos_x_player +20< (pos_pista_x) ){
                    pos_pista_x -= bgSpeedX;
                    }
                }

                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                    // Aumentar a velocidade gradualmente ate a velocidade maxima
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
                    // Reduzir a velocidade gradualmente quando a tecla não esta pressionada
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
                    // Reduzir a velocidade gradualmente quando a tecla não esta pressionada
                    if (bgSpeed > 0.4) {
                        bgSpeedX = 0.4;
                        pos_pista_x -= bgSpeedX;
                        bgSpeed -= acel * 1.2; // A desaceleração e mais lenta
                        if (bgSpeed <= 0) {
                            bgSpeed = 0;
                        }
                    } else {
                        pos_pista_x -= bgSpeedX;
                    }
                    if (SpriteTimer > 25) {
                        estado_player = 2;
                        if (SpriteTimer > 50) {
                        estado_player = 3;
                        SpriteTimer = 51;
                        }
                    } else {
                        estado_player = 1;
                    }
                } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                    SpriteTimer -= bgSpeedX;
                    // Reduzir a velocidade gradualmente quando a tecla não esta pressionada
                    if (bgSpeed > 0.4) {
                        bgSpeedX = 0.4;
                        pos_pista_x += bgSpeedX;
                        bgSpeed -= acel * 1.2; // A desaceleração e mais lenta
                        if (bgSpeed <= 0) {
                            bgSpeed = 0;
                        }
                    } else {
                        bgSpeedX = bgSpeed;
                        pos_pista_x += bgSpeedX;
                    }
                    if (SpriteTimer < -25) {
                        estado_player = 5;
                        if (SpriteTimer < -50) {
                        SpriteTimer = -51;
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

                // Gera obstaculos periodicamente
                if (obstacleSpawnTimer >= obstacleSpawnInterval) {
                    SpawnObstaculo(pos_pista_x , pos_pista_y, Pista.width * pistaScale, obstacleTexture, pistaScale, Pista);
                    obstacleSpawnTimer = 0.0f; // Reinicia o temporizador
                    obstacleSpawnInterval = 0.2f;// Define o tempo paar spawnar como 2
                }

                // Atualiza e desenha obstaculos
                UpdateEDrawObstaculo(&bgSpeed, bgSpeedX, estado_player, pos_pista_x, Pista, Jogador, pos_x_player , AreaJogador);

                // Logica para detectar clique no botão de saida na tela de ranking
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    if (CheckCollisionPointRec(mousePoint, (Rectangle){s_width / 2 - buttonExit.width / 2, 50, buttonExit.width, buttonExit.height})) {
                        screenAtual = MENUS;
                    }
                }

                // Atualização do velocimetro
                DrawVelocimetro(bgSpeed, maxSpeed);

                break;

            case R_screen:
                DrawRScreen(buttonExit);
                // Logica para detectar clique no botão de saida na tela de ranking
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

    UnloadMusicStream(music);
    CloseAudioDevice();

    // Fechamento da janela
    CloseWindow();

    return 0;
}

// Função para inicializar os obstaculos
void InitObstaculo(Texture2D obstacleTexture) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        // Inicializa a posição do obstaculo como (0, 0)
        obstaculos[i].position = (Vector2){0, 0};
        // Define a textura do obstaculo como a textura carregada
        obstaculos[i].texture = obstacleTexture;
        // Define o obstaculo como inativo inicialmente
        obstaculos[i].Ativo = false;
    }
}

// Função para gerar um obstaculo em uma posição especifica da pista
void SpawnObstaculo(float pistaX, float pistaY, float pistaWidth, Texture2D obstacleTexture, float pistaScale, Texture2D Pista) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        // Verifica se ha algum obstaculo inativo para ser ativado
        if (!obstaculos[i].Ativo) {
            // Define a posição do obstaculo na pista, na parte superior visivel
            obstaculos[i].position = (Vector2){ pistaX + GetRandomValue(56, pistaWidth - obstacleTexture.width - 56), pistaY - ((Pista.height * pistaScale)*(5*(i+1)))};
            // Ativa o obstaculo
            obstaculos[i].Ativo = true;
            break; // Sai do loop apos ativar um obstaculo
        }
    }
}

void UpdateEDrawObstaculo(float *bgSpeed, float bgSpeedX, int estado_player ,float posX , Texture2D Pista , Texture2D Jogador, int pos_x_player, Rectangle AreaJogador) {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {

        if (obstaculos[i].Ativo) {
            // Atualiza a posição vertical do obstaculo com base na velocidade da pista
            obstaculos[i].position.y += *bgSpeed ;

            // Se não na borda aceita movimento
            if ( pos_x_player + 20 > posX && pos_x_player  + Jogador.width -20< posX + Pista.width){            
                // Atualiza a posição horizontal do obstaculo com base no movimento lateral do jogador
                if (estado_player <= 3 && estado_player != 0) { // Move para a direita
                    obstaculos[i].position.x -= bgSpeedX * 0.5;
                } else if (estado_player >= 4) { // Move para a esquerda
                    obstaculos[i].position.x += bgSpeedX * 0.5;
                }
            }
            // Verifica se o obstaculo saiu da tela
            if (obstaculos[i].position.y > s_height) {
                obstaculos[i].Ativo = false;
            } else {
                // Desenha o obstaculo na tela
                DrawTexture(obstaculos[i].texture, obstaculos[i].position.x, obstaculos[i].position.y, WHITE);
            }
            // Desenha o círculo de colisão (invisível)
            float obstacleRadius = obstaculos[i].texture.width / 2.0f;
            Vector2 obstacleCenter = {obstaculos[i].position.x + obstacleRadius, obstaculos[i].position.y + obstacleRadius};
            DrawCircleV(obstacleCenter, obstacleRadius, Fade(RED, 0.5)); // Descomentar para visualizar o círculo de colisão

            // Verifica a colisão
            if (CheckCollisionCircleRec(obstacleCenter, obstacleRadius, AreaJogador)) {

                // Reduz a velocidade ao colidir com um obstáculo
                *bgSpeed -= 0.1f; // Diminui a velocidaed


                if (*bgSpeed <= 0.1) {
                    *bgSpeed = 0.1;
                }
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

void DrawPScreen(Texture2D buttonExit, Texture2D Pista, Texture2D Background, Texture2D BG,Texture2D Sky, Texture2D BG2, Texture2D Jogador, Texture2D Jogador_D1, Texture2D Jogador_D2, Texture2D Jogador_E1, Texture2D Jogador_E2, float *pos_pista_x, float *pos_pista_y, float *bgSpeed, float bgSpeedX, int *estado_player, float *SpriteTimer, float scale,  int pos_x_player, Rectangle AreaJogador, Texture2D Grama) { 
    int pos_BG_y = 300;

    ClearBackground(DARKPURPLE);
    float currentY = *pos_pista_y; // Define onde começa a desenhar a pista

    // Desenhar as camadas da pista para criar a repetição da imagem
    while (currentY > pos_BG_y - Pista.height * scale) {
        // Calcular a posição x para centralizar a pista
        float posX = *pos_pista_x;

        // Desenha a textura da pista com a escala
        DrawTextureEx(Grama, (Vector2){posX - Grama.width, currentY}, 0.0f, 1.0f, WHITE);
        DrawTextureEx(Grama, (Vector2){posX + (Pista.width*scale), currentY}, 0.0f, 1.0f, WHITE);

        DrawTextureEx(Pista, (Vector2){posX, currentY}, 0.0f, scale, WHITE);


        // Atualiza a posição vertical para a proxima camada
        currentY -= Pista.height * scale;
    }

    // Atualiza e desenha obstaculos
    UpdateEDrawObstaculo( *&bgSpeed , bgSpeedX, *estado_player, *pos_pista_x , Pista, Jogador, pos_x_player, AreaJogador); 

    // Desenha o fundo depois
    DrawTextureEx(Sky , (Vector2){0, -300}, 0, 0.9, WHITE);
    DrawTextureEx(BG2 , (Vector2){-100, 75}, 0, 0.8, WHITE);    
    DrawTextureEx(BG2 , (Vector2){600, 115}, 0, 0.8, WHITE);
    DrawTextureEx(BG , (Vector2){0, 0}, 0, 0.8, WHITE);
    DrawTextureEx(Background , (Vector2){0, 300}, 0, 1, WHITE);

    // Desenha o gotão de saida
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

    // Medindo as dimensoes do texto
    float textWidth = MeasureText(TextFormat("%.0f Km/h", Kmh), 25);

    // Definindo a posição inicial do velocimetro (canto superior esquerdo)
    int posX = 800;
    int posY = 665;
    int centerPosX = posX + 150;
    int centerPosY = posY + 150;

    // Definindo o angulo do ponteiro começando em 220 
    float angulo = 220 - Kmh * 1.25; 

    // Desenhando o circulo do velocimetro (centro)
    DrawCircle(centerPosX, centerPosY, 70, Fade(BLACK, 0.9));

    // Desenhando o contorno cinza escuro ao redor do velocimetro
    DrawRing((Vector2){ centerPosX, centerPosY }, 72, 67, 0, 360, 20, DARKGRAY);

    // Desenhando a linha do velocimetro
    DrawLine(centerPosX, centerPosY, centerPosX + 50 * cos(angulo * DEG2RAD), centerPosY - 50 * sin(angulo * DEG2RAD), RED);

    // Posição da bola vermelha (indicador da velocidade)
    float indicatorX = centerPosX + 50 * cos(angulo * DEG2RAD);
    float indicatorY = centerPosY - 50 * sin(angulo * DEG2RAD);

    // Desenhando o indicador da velocidade atual (bola vermelha)
    DrawCircle(indicatorX, indicatorY, 5, RED);

    // Desenhando o texto com a velocidade
    DrawText(TextFormat("%.0f Km/h", Kmh), centerPosX - textWidth / 2 +20, centerPosY + 40, 15, GREEN);
}

