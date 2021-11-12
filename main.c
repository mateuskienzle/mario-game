#include "raylib.h"
#include<stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_PLATAFORMS 500


typedef struct {
    Rectangle frameRec_forward;

} FRAMES;


typedef struct {
    Rectangle Position;

} Plataform;


typedef struct {
    Plataform plataforms[MAX_PLATAFORMS];
    int num_plataforms;

} Map_1;


typedef struct {
    Vector2 Position;
    Vector2 Size;
    Vector2 pow_button;
    Vector2 enemy_return_1;
    Vector2 enemy_return_2;
    Vector2 enemy_spawn_1;
    Vector2 enemy_spawn_2;

    int Ammo;
    int Lives;
    int Points;
} Player;

void init(Player*);
void update(Player*);

void draw(Player, Texture2D , Rectangle, Texture2D, Rectangle, Texture2D, Rectangle, Texture2D,
           Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle,
           Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D,
           Map_1);

void init_Map_1(Map_1 *Map);

int main(void) {

    Player play;
    Map_1 load;
    init_Map_1(&load);


    FILE *arq = fopen("map1_mario.txt", "r");
    int col=0, lin=0;

    while(!feof(arq)) {
        switch(fgetc(arq)){
            case 'p':
                load.plataforms[load.num_plataforms].Position.x = col*10;
                load.plataforms[load.num_plataforms].Position.y = lin*25;
                load.num_plataforms++;
                break;
            case 'm':
                play.Position.x = col*10;
                play.Position.y = lin*25;
                break;
            case 'b':
                play.pow_button.x = col*10;
                play.pow_button.y = lin*25;
                break;
            case 's':
                play.enemy_spawn_1.x = col*10;
                play.enemy_spawn_1.y = lin*25;
                break;
            case 'e':
                play.enemy_spawn_2.x = col*10;
                play.enemy_spawn_2.y = lin*25;
                break;
            case '1':
                play.enemy_return_1.x = col*10;
                play.enemy_return_1.y = lin*25;
                break;
            case '2':
                play.enemy_return_2.x = col*10;
                play.enemy_return_2.y = lin*25;
                break;
            case '\n':
                col=-1;
                lin++;
        }
        col++;
    }
    fclose(arq);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu jogador");

    //Mario walking forward
    Texture2D mario_forward = LoadTexture("images/mario/forward.png");
    Rectangle frameRec_forward = { 0.0f, 0.0f, (float)mario_forward.width/4, (float)mario_forward.height };
    int currentFrame_forward = 0;
    int framesCounter_forward = 0;

    //Mario walking backward
    Texture2D mario_backward = LoadTexture("images/mario/backward.png");
    Rectangle frameRec_backward = { 0.0f, 0.0f, (float)mario_backward.width/4, (float)mario_backward.height };
    int currentFrame_backward = 0;
    int framesCounter_backward = 0;

    //Mario jumping forward
    Texture2D mario_jumping_forward = LoadTexture("images/mario/jumping_forward.png");
    Rectangle frameRec_jumping_forward = { 1.0f, 1.0f, (float)mario_jumping_forward.width/1, (float)mario_jumping_forward.height };
    int currentFrame_jumping = 0;
    int framesCounter_jumping = 0;

    //Mario jumping backward
    Texture2D mario_jumping_backward = LoadTexture("images/mario/jumping_backward.png");
    Rectangle frameRec_jumping_backward = { 0.0f, 0.0f, (float)mario_jumping_backward.width/1, (float)mario_jumping_backward.height };

    //Pow button
    Texture2D pow_button = LoadTexture("images/sprite_geral.png");
    Rectangle pow_button_sourceRec = { 133.0f, 243.0f, 16, 16 };
    Rectangle pow_button_destRec = { play.pow_button.x, play.pow_button.y, 16*2.0f, 16*2.0f };

    //Plataforms
    Texture2D plataform = LoadTexture("images/sprite_geral.png");
    Rectangle plataform_sourceRec = { 133.0f, 278.0f, 11, 8 };

    //Floor
    Texture2D floor = LoadTexture("images/sprite_geral.png");
    Rectangle floor_sourceRec = { 5.0f, 293.0f, 15, 15 };
    Rectangle floor_destRec = {0.0f, 667.0f, 15*2.0f, 15*2.0f };

    //Enemy return 1
    Texture2D enemy_return_1 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_return_1_sourceRec = { 11.0f, 215.0f, 32, 18 };
    Rectangle enemy_return_1_destRec = { play.enemy_return_1.x, play.enemy_return_1.y, 32*2.0f, 18*2.0f };

    //Enemy return 2
    Texture2D enemy_return_2 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_return_2_sourceRec = { 11.0f, 258.0f, 32, 18 };
    Rectangle enemy_return_2_destRec = { play.enemy_return_2.x, play.enemy_return_2.y, 32*2.0f, 18*2.0f };

    //Enemy spawn 1
    Texture2D enemy_spawn_1 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_spawn_1_sourceRec = { 51.0f, 246.0f, 48, 30 };
    Rectangle enemy_spawn_1_destRec = { play.enemy_spawn_1.x, play.enemy_spawn_1.y, 48*2.0f, 30*2.0f };

    //Enemy spawn 2
    Texture2D enemy_spawn_2 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_spawn_2_sourceRec = { 51.0f, 203.0f, 48, 30 };
    Rectangle enemy_spawn_2_destRec = { play.enemy_spawn_2.x, play.enemy_spawn_2.y, 48*2.0f, 30*2.0f };

    //Speed ​​at which Mario's movement frames are switched
    int framesSpeed = 16;

    init(&play);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key

        if ((IsKeyDown(KEY_LEFT) && play.Position.x+play.Size.x >= 0)&&(IsKeyDown(KEY_RIGHT) && play.Position.x+play.Size.x < SCREEN_WIDTH)) {
            framesCounter_forward = 0;
            currentFrame_forward = 0;
            frameRec_forward.x = (float)currentFrame_forward*(float)mario_forward.width/4;;
        }

        else if (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Size.x <= SCREEN_WIDTH) {
            framesCounter_forward++;
        }
        else if(IsKeyDown(KEY_LEFT) && play.Position.x+play.Size.x >= 0) {
            framesCounter_backward++;
        }
        else if(IsKeyDown(KEY_UP) && (play.Position.y+play.Size.y >= 0)) {
            framesCounter_jumping++;
        }

        else {
            framesCounter_forward = 0;
            currentFrame_forward = 0;
            frameRec_forward.x = (float)currentFrame_forward*(float)mario_forward.width/4;

            framesCounter_backward = 0;
            currentFrame_backward = 0;
            frameRec_backward.x = (float)currentFrame_backward*(float)mario_backward.width/4;

            // acho que essa parte é desnecessária
            framesCounter_jumping = 0;
            currentFrame_jumping = 0;
            frameRec_jumping_forward.y = (float)currentFrame_jumping*(float)mario_jumping_forward.width/1;
            frameRec_jumping_backward.y = (float)currentFrame_jumping*(float)mario_jumping_backward.width/1;

        }

        if (framesCounter_forward >= (60/framesSpeed)){

            framesCounter_forward = 0;
            currentFrame_forward++;

            if (currentFrame_forward > 3) currentFrame_forward = 0;{

            frameRec_forward.x = (float)currentFrame_forward*(float)mario_forward.width/4;
            }
        }
        if (framesCounter_backward >= (60/framesSpeed)){
            framesCounter_backward = 0;
            currentFrame_backward++;

            if (currentFrame_backward > 3) currentFrame_backward = 0;{

            frameRec_backward.x = (float)currentFrame_backward*(float)mario_backward.width/4;
            }
        }

        update(&play);

        draw(play, mario_forward, frameRec_forward, mario_backward, frameRec_backward, mario_jumping_forward,
             frameRec_jumping_forward, mario_jumping_backward, frameRec_jumping_backward, pow_button,
             pow_button_sourceRec, pow_button_destRec, floor, floor_sourceRec, floor_destRec, enemy_return_1,
             enemy_return_1_sourceRec, enemy_return_1_destRec, enemy_return_2, enemy_return_2_sourceRec, enemy_return_2_destRec,
             enemy_spawn_1, enemy_spawn_1_sourceRec, enemy_spawn_1_destRec, enemy_spawn_2, enemy_spawn_2_sourceRec, enemy_spawn_2_destRec,
             plataform, load);

    }

    CloseWindow();

    return 0;
}

// Function that determines Mario startup
void init (Player *play) {

    play->Position = (Vector2) {
        play->Position.x, play->Position.y
    };
    play->Size = (Vector2) {
        50, 50
    };
}

// Function that sets Mario's scroll speed
void update (Player *play) {

    if (play->Position.y+80 < 650) {
        // gravity
        play->Position.y += 4.0f;;
    }
    if (IsKeyDown(KEY_RIGHT) && play->Position.x+play->Size.x < SCREEN_WIDTH) {
        play->Position.x += 4.0f;
    }
    if (IsKeyDown(KEY_LEFT) && play->Position.x >= 0) {
        play->Position.x -= 4.0f;
    }
    if (IsKeyDown(KEY_UP) && play->Position.y >= 0) {
        play->Position.y -= 8.0f;
    }

}

void init_Map_1(Map_1 *Map){
    int i=0;
    Map->num_plataforms=0;
    for(i=0;i<MAX_PLATAFORMS;i++){
        Map->plataforms[i].Position.width = 10;
        Map->plataforms[i].Position.height = 20;
    }
}

void draw (Player play, Texture2D mario_forward, Rectangle frameRec_forward, Texture2D mario_backward,
           Rectangle frameRec_backward, Texture2D mario_jumping_forward, Rectangle frameRec_jumping_forward,
           Texture2D mario_jumping_backward, Rectangle frameRec_jumping_backward, Texture2D pow_button,
           Rectangle pow_button_sourceRec, Rectangle pow_button_destRec, Texture2D floor, Rectangle floor_sourceRec,
           Rectangle floor_destRec, Texture2D enemy_return_1, Rectangle enemy_return_1_sourceRec, Rectangle enemy_return_1_destRec,
           Texture2D enemy_return_2, Rectangle enemy_return_2_sourceRec, Rectangle enemy_return_2_destRec, Texture2D enemy_spawn_1,
           Rectangle enemy_spawn_1_sourceRec, Rectangle enemy_spawn_1_destRec, Texture2D enemy_spawn_2, Rectangle enemy_spawn_2_sourceRec,
           Rectangle enemy_spawn_2_destRec, Texture2D plataform, Map_1 load) {


    BeginDrawing();

    ClearBackground(BLACK);

    char control = 'r';
    int i;

    //plataforms
    //DrawTexturePro(plataform, plataform_sourceRec, plataform_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);

    for(i=0;i<load.num_plataforms;i++){
        DrawRectangleRec(load.plataforms[i].Position, BLUE);
    }

    //pow button
    static int framesource=0;
    static int contador=0.5*60;

    if (contador!=0)
        contador--;
    else {
        contador=0.5*60;
        framesource++;
        if(framesource>2)
            framesource=0;
    }
    pow_button_sourceRec.x+=20*framesource;

    DrawTexturePro(pow_button, pow_button_sourceRec, pow_button_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);


    //floor
    do {
    DrawTexturePro(floor, floor_sourceRec, floor_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);
    floor_destRec.x+=16;
    }while (floor_destRec.x < SCREEN_WIDTH);

    // Enemy return 1
    DrawTexturePro(enemy_return_1, enemy_return_1_sourceRec, enemy_return_1_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);

    // Enemy return 2
    DrawTexturePro(enemy_return_2, enemy_return_2_sourceRec, enemy_return_2_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);

    // Enemy spawn 1
    DrawTexturePro(enemy_spawn_1, enemy_spawn_1_sourceRec,enemy_spawn_1_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);

    // Enemy spawn 2
    DrawTexturePro(enemy_spawn_2, enemy_spawn_2_sourceRec,enemy_spawn_2_destRec, (Vector2){ 0, 0 }, (float)0.0f, WHITE);

    // Mario
    if ((IsKeyDown(KEY_LEFT) && play.Position.x+play.Size.x >= 0)&&(IsKeyDown(KEY_RIGHT) && play.Position.x+play.Size.x < SCREEN_WIDTH)) {
        DrawTextureRec(mario_forward, frameRec_forward, play.Position, WHITE);
    }
    else if ((IsKeyDown(KEY_UP) && play.Position.y+play.Size.y < SCREEN_HEIGHT) && (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Size.x < SCREEN_WIDTH)) {
        DrawTextureRec(mario_jumping_forward, frameRec_jumping_forward, play.Position, WHITE);
    }

    else if ((IsKeyDown(KEY_UP) && play.Position.y+play.Size.y < SCREEN_HEIGHT) && (IsKeyDown(KEY_LEFT) && play.Position.x+play.Size.x >= 0)) {
        DrawTextureRec(mario_jumping_backward, frameRec_jumping_forward, play.Position, WHITE);
    }

    else if (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Size.x < SCREEN_WIDTH){
        DrawTextureRec(mario_forward, frameRec_forward, play.Position, WHITE);
        control = 'r';
    }

    else if (IsKeyDown(KEY_LEFT) && play.Position.x+play.Size.x >= 0) {
        DrawTextureRec(mario_backward, frameRec_backward, play.Position, WHITE);
        control = 'l';
    }

    else if (IsKeyDown(KEY_UP)){

        if (play.Position.y+play.Size.y < SCREEN_HEIGHT && control == 'r'){
        DrawTextureRec(mario_jumping_forward, frameRec_jumping_forward, play.Position, WHITE);
        }

        else if (play.Position.y+play.Size.y < SCREEN_HEIGHT && control == 'l'){
        DrawTextureRec(mario_jumping_backward, frameRec_jumping_backward, play.Position, WHITE);
        }
    }

    else{
        if(control == 'r'){
            DrawTextureRec(mario_forward, frameRec_forward, play.Position, WHITE);
        }
        else if (control == 'l'){
            DrawTextureRec(mario_backward, frameRec_backward, play.Position, WHITE);
        }
        else{
            DrawTextureRec(mario_forward, frameRec_forward, play.Position, WHITE);
        }
    }

    EndDrawing();
}
