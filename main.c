#include "raylib.h"
#include<stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_PLATAFORMS 500

#define V2n (Vector2){ 0, 0 }

typedef struct {
    Rectangle Position;

} Plataform;

bool isGrounded;
bool jump;
int jump_timer;

typedef struct {
    Plataform plataforms[MAX_PLATAFORMS];
    int num_plataforms;
    Vector2 pow_button;
    Vector2 enemy_return_1;
    Vector2 enemy_return_2;
    Vector2 enemy_spawn_1;
    Vector2 enemy_spawn_2;

} Map_1;


typedef struct {
    Rectangle Position;
    int Ammo;
    int Lives;
    int Points;
} Player;



void init(Player*);
void update(Player*);

void draw(Player, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D,
           Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle,
           Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Map_1,
           Rectangle, Rectangle, Texture2D, Rectangle, Rectangle, Texture2D, Rectangle, Rectangle);

void init_Map_1(Map_1 *Map);
void plataform_collision(Rectangle, Rectangle, Map_1, Player*);

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
                load.pow_button.x = col*10;
                load.pow_button.y = lin*25;
                break;
            case 's':
                load.enemy_spawn_1.x = col*10;
                load.enemy_spawn_1.y = lin*25;
                break;
            case 'e':
                load.enemy_spawn_2.x = col*10;
                load.enemy_spawn_2.y = lin*25;
                break;
            case '1':
                load.enemy_return_1.x = col*10;
                load.enemy_return_1.y = lin*25;
                break;
            case '2':
                load.enemy_return_2.x = col*10;
                load.enemy_return_2.y = lin*25;
                break;
            case '\n':
                col=-1;
                lin++;
        }
        col++;
    }
    fclose(arq);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My player");

    //Mario walking forward
    Texture2D mario_forward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_forward_sourceRec = { 0.0f, 633.0f, 64/4, 21 };
    Rectangle mario_forward_destRec = { play.Position.x, play.Position.y, (64/4)*2, 21*2 };
    int currentFrame_forward = 0;
    int framesCounter_forward = 0;

    //Mario walking backward
    Texture2D mario_backward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_backward_sourceRec = { 0.0f, 603.0f, 64/4, 21 };
    Rectangle mario_backward_destRec = { play.Position.x, play.Position.y, (64/4)*2, 21*2 };
    int currentFrame_backward = 0;
    int framesCounter_backward = 0;

    //Mario jumping forward
    Texture2D mario_jumping_forward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_jumping_forward_sourceRec = { 93.0f, 633.0f, 15, 21 };
    Rectangle mario_jumping_forward_destRec = { play.Position.x, play.Position.y, 15*2, 21*2 };


    //Mario jumping backward
    Texture2D mario_jumping_backward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_jumping_backward_sourceRec = { 93.0f, 603.0f, 15, 21 };
    Rectangle mario_jumping_backward_destRec = { play.Position.x, play.Position.y, 15*2, 21*2 };


    //Mario falling forward
    Texture2D mario_falling_forward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_falling_forward_sourceRec = { 73.0f, 632.0f, 15, 21 };
    Rectangle mario_falling_forward_destRec = { play.Position.x, play.Position.y, 15*2, 21*2 };

    //Mario falling backward
    Texture2D mario_falling_backward = LoadTexture("images/sprite_geral.png");
    Rectangle mario_falling_backward_sourceRec = { 73.0f, 602.0f, 15, 21 };
    Rectangle mario_falling_backward_destRec = { play.Position.x, play.Position.y, 15*2, 21*2 };

    //Pow button
    Texture2D pow_button = LoadTexture("images/sprite_geral.png");
    Rectangle pow_button_sourceRec = { 133.0f, 243.0f, 16, 16 };
    Rectangle pow_button_destRec = { load.pow_button.x, load.pow_button.y, 16*2.0f, 16*2.0f };

    //Floor
    Texture2D floor = LoadTexture("images/sprite_geral.png");
    Rectangle floor_sourceRec = { 5.0f, 293.0f, 15, 15 };
    Rectangle floor_destRec = {0.0f, 667.0f, 15*2.0f, 15*2.0f };

    //Enemy return 1
    Texture2D enemy_return_1 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_return_1_sourceRec = { 11.0f, 215.0f, 32, 18 };
    Rectangle enemy_return_1_destRec = { load.enemy_return_1.x, load.enemy_return_1.y, 32*2.0f, 18*2.0f };

    //Enemy return 2
    Texture2D enemy_return_2 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_return_2_sourceRec = { 11.0f, 258.0f, 32, 18 };
    Rectangle enemy_return_2_destRec = { load.enemy_return_2.x, load.enemy_return_2.y, 32*2.0f, 18*2.0f };

    //Enemy spawn 1
    Texture2D enemy_spawn_1 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_spawn_1_sourceRec = { 51.0f, 246.0f, 48, 30 };
    Rectangle enemy_spawn_1_destRec = { load.enemy_spawn_1.x, load.enemy_spawn_1.y, 48*2.0f, 30*2.0f };

    //Enemy spawn 2
    Texture2D enemy_spawn_2 = LoadTexture("images/sprite_geral.png");
    Rectangle enemy_spawn_2_sourceRec = { 51.0f, 203.0f, 48, 30 };
    Rectangle enemy_spawn_2_destRec = { load.enemy_spawn_2.x, load.enemy_spawn_2.y, 48*2.0f, 30*2.0f };


    //Enemies
    Rectangle turtle_1 = { 97.f, 130.f, 40, 40};
    Rectangle turtle_2 = {1069.f, 130.f, 40, 40};



    //Speed ​​at which Mario's movement frames are switched
    int framesSpeed = 16;

    init(&play);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key

        mario_forward_destRec = (Rectangle) { play.Position.x, play.Position.y, 15*2, 21*2 };
        mario_backward_destRec = (Rectangle) { play.Position.x, play.Position.y, 15*2, 21*2 };
        mario_jumping_backward_destRec = (Rectangle){ play.Position.x, play.Position.y, 15*2, 21*2 };
        mario_jumping_forward_destRec = (Rectangle){ play.Position.x, play.Position.y, 15*2, 21*2 };
        mario_falling_backward_destRec = (Rectangle){ play.Position.x, play.Position.y, 15*2, 21*2 };
        mario_falling_forward_destRec = (Rectangle){ play.Position.x, play.Position.y, 15*2, 21*2 };

        if ((IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0)&&(IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width < SCREEN_WIDTH)) {
            framesCounter_forward = 0;
            currentFrame_forward = 0;
            mario_forward_sourceRec.x = (float)currentFrame_forward*(float)64/4;
        }

        else if (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width <= SCREEN_WIDTH) {
            framesCounter_forward++;
        }
        else if(IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0) {
            framesCounter_backward++;
        }


        else {
            framesCounter_forward = 0;
            currentFrame_forward = 0;
            mario_forward_sourceRec.x = (float)currentFrame_forward*(float)(64/4);

            framesCounter_backward = 0;
            currentFrame_backward = 0;
            mario_backward_sourceRec.x = (float)currentFrame_backward*(float)64/4;

        }

        if (framesCounter_forward >= (60/framesSpeed)){

            framesCounter_forward = 0;
            currentFrame_forward++;

            if (currentFrame_forward > 3) currentFrame_forward = 0;{

            mario_forward_sourceRec.x = (float)currentFrame_forward*(float)64/4;
            }
        }
        if (framesCounter_backward >= (60/framesSpeed)){

            framesCounter_backward = 0;
            currentFrame_backward++;

            if (currentFrame_backward > 3) currentFrame_backward = 0;{

            mario_backward_sourceRec.x = (float)currentFrame_backward*(float)64/4;
            }
        }

        update(&play);

        draw(play, mario_forward, mario_forward_sourceRec, mario_forward_destRec, mario_backward, mario_backward_sourceRec, mario_backward_destRec, mario_jumping_forward,
             mario_jumping_forward_sourceRec, mario_jumping_forward_destRec, mario_jumping_backward, mario_jumping_backward_sourceRec, mario_jumping_backward_destRec, pow_button,
             pow_button_sourceRec, pow_button_destRec, floor, floor_sourceRec, floor_destRec, enemy_return_1,
             enemy_return_1_sourceRec, enemy_return_1_destRec, enemy_return_2, enemy_return_2_sourceRec, enemy_return_2_destRec,
             enemy_spawn_1, enemy_spawn_1_sourceRec, enemy_spawn_1_destRec, enemy_spawn_2, enemy_spawn_2_sourceRec, enemy_spawn_2_destRec,
             load, turtle_1, turtle_2, mario_falling_forward, mario_falling_forward_sourceRec,
             mario_falling_forward_destRec, mario_falling_backward, mario_falling_backward_sourceRec,
             mario_falling_backward_destRec);

        plataform_collision(mario_forward_destRec, mario_backward_destRec,load, &play);


    }

    CloseWindow();

    return 0;
}

// Function that determines Mario startup
void init (Player *play) {

    play->Position = (Rectangle) {
        play->Position.x, play->Position.y, 0 , 0
    };

}

// Function that sets Mario's scroll speed
void update (Player *play) {

    if (IsKeyDown(KEY_RIGHT) && play->Position.x+play->Position.width <= SCREEN_WIDTH-35) {
        play->Position.x += 4.0f;
    }
    if (IsKeyDown(KEY_LEFT) && play->Position.x+play->Position.width >= 0) {
        play->Position.x -= 4.0f;
    }


    if (IsKeyPressed(KEY_UP) && (isGrounded || (play->Position.y+play->Position.height <= SCREEN_HEIGHT && play->Position.y+play->Position.height >= SCREEN_HEIGHT-78))
         && play->Position.y+play->Position.height >= 0) {

       jump = true;

    }

    if (jump == true){
        jump_timer +=1;
        if(jump_timer < 40){
            play->Position.y -=10;
        }
        else {
            play->Position.y +=1;
        }
    }

    if (jump_timer >=50){

        jump_timer = 0;
        jump = false;
    }


    if(!isGrounded){
        if(play->Position.y+play->Position.height <= SCREEN_HEIGHT-78) {
        // gravity
        play->Position.y += 6.0f;

        }
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


void draw (Player play, Texture2D mario_forward, Rectangle mario_forward_sourceRec, Rectangle mario_forward_destRec, Texture2D mario_backward,
           Rectangle mario_backward_sourceRec, Rectangle mario_backward_destRec, Texture2D mario_jumping_forward, Rectangle mario_jumping_forward_sourceRec,
           Rectangle mario_jumping_forward_destRec, Texture2D mario_jumping_backward, Rectangle mario_jumping_backward_sourceRec, Rectangle mario_jumping_backward_destRec, Texture2D pow_button,
           Rectangle pow_button_sourceRec, Rectangle pow_button_destRec, Texture2D floor, Rectangle floor_sourceRec,
           Rectangle floor_destRec, Texture2D enemy_return_1, Rectangle enemy_return_1_sourceRec, Rectangle enemy_return_1_destRec,
           Texture2D enemy_return_2, Rectangle enemy_return_2_sourceRec, Rectangle enemy_return_2_destRec, Texture2D enemy_spawn_1,
           Rectangle enemy_spawn_1_sourceRec, Rectangle enemy_spawn_1_destRec, Texture2D enemy_spawn_2, Rectangle enemy_spawn_2_sourceRec,
           Rectangle enemy_spawn_2_destRec, Map_1 load, Rectangle turtle_1, Rectangle turtle_2, Texture2D mario_falling_forward, Rectangle mario_falling_forward_sourceRec,
           Rectangle mario_falling_forward_destRec, Texture2D mario_falling_backward, Rectangle mario_falling_backward_sourceRec,
           Rectangle mario_falling_backward_destRec) {


    BeginDrawing();

    ClearBackground(BLACK);


    static char control='r';
    int i;

    // Enemies
    DrawRectangleRec(turtle_1, YELLOW);
    DrawRectangleRec(turtle_2, MAROON);


    //plataforms
    for(i=0;i<load.num_plataforms;i++){
        DrawRectangleRec(load.plataforms[i].Position, BLUE);
    }

    //pow button
    static int framesource=0;
    static int counter=0.5*60;

    if (counter!=0)
        counter--;
    else {
        counter=0.5*60;
        framesource++;
        if(framesource>2)
            framesource=0;
    }
    pow_button_sourceRec.x+=20*framesource;



    DrawTexturePro(pow_button, pow_button_sourceRec, pow_button_destRec, V2n, (float)0.0f, WHITE);


    //floor
    do {
    DrawTexturePro(floor, floor_sourceRec, floor_destRec, V2n, (float)0.0f, WHITE);
    floor_destRec.x+=16;
    } while (floor_destRec.x < SCREEN_WIDTH);

    // Enemy return 1
    DrawTexturePro(enemy_return_1, enemy_return_1_sourceRec, enemy_return_1_destRec,V2n, 0.0f, WHITE);

    // Enemy return 2
    DrawTexturePro(enemy_return_2, enemy_return_2_sourceRec, enemy_return_2_destRec, V2n, 0.0f, WHITE);

    // Enemy spawn 1
    DrawTexturePro(enemy_spawn_1, enemy_spawn_1_sourceRec, enemy_spawn_1_destRec, V2n, 0.0f, WHITE);

    // Enemy spawn 2
    DrawTexturePro(enemy_spawn_2, enemy_spawn_2_sourceRec, enemy_spawn_2_destRec, V2n, 0.0f, WHITE);

    // Mario
    if ((IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0)&&(IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width < SCREEN_WIDTH)) {
      DrawTexturePro(mario_forward, mario_forward_sourceRec, mario_forward_destRec, V2n, 0.0f, WHITE);
    }
    else if (((IsKeyPressed(KEY_UP) && play.Position.y+play.Position.height < SCREEN_HEIGHT) && (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width < SCREEN_WIDTH)) || jump == true && control == 'r') {
        DrawTexturePro(mario_jumping_forward, mario_jumping_forward_sourceRec, mario_backward_destRec, V2n, 0.0f, WHITE);
    }

    else if (((IsKeyPressed(KEY_UP)  && play.Position.y+play.Position.height < SCREEN_HEIGHT) && (IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0)) || jump == true && control == 'l') {
        DrawTexturePro(mario_jumping_backward, mario_jumping_backward_sourceRec, mario_jumping_backward_destRec, V2n, 0.0f, WHITE);
    }

    else if ((!isGrounded && !jump && control == 'l' && play.Position.y+play.Position.height < SCREEN_HEIGHT-78) && (IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0)) {
        DrawTexturePro(mario_falling_backward, mario_falling_backward_sourceRec, mario_falling_backward_destRec, V2n, 0.0f, WHITE);
    }

    else if ((!isGrounded && !jump && control == 'r' && play.Position.y+play.Position.height < SCREEN_HEIGHT-78) && (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width < SCREEN_WIDTH)) {
       DrawTexturePro(mario_falling_forward, mario_falling_forward_sourceRec, mario_falling_forward_destRec, V2n, 0.0f, WHITE);
    }

    else if (IsKeyDown(KEY_RIGHT) && play.Position.x+play.Position.width < SCREEN_WIDTH){
        DrawTexturePro(mario_forward, mario_forward_sourceRec, mario_forward_destRec, V2n, 0.0f, WHITE);
        control = 'r';
    }

    else if (IsKeyDown(KEY_LEFT) && play.Position.x+play.Position.width >= 0) {
        DrawTexturePro(mario_backward, mario_backward_sourceRec, mario_backward_destRec, V2n, 0.0f, WHITE);
        control = 'l';
    }


    else if ((IsKeyPressed(KEY_UP) && play.Position.y+play.Position.height < SCREEN_HEIGHT && control == 'r') || jump == true && control == 'r'){
        DrawTexturePro(mario_jumping_forward, mario_jumping_forward_sourceRec, mario_jumping_forward_destRec, V2n, 0.0f, WHITE);
    }

    else if ((IsKeyPressed(KEY_UP) && play.Position.y+play.Position.height < SCREEN_HEIGHT && control == 'l') || jump == true && control == 'l'){
        DrawTexturePro(mario_jumping_backward, mario_jumping_backward_sourceRec, mario_jumping_backward_destRec, V2n, 0.0f, WHITE);
    }

    else if(!isGrounded && !jump && control == 'r' && play.Position.y+play.Position.height < SCREEN_HEIGHT-78){
        DrawTexturePro(mario_falling_forward, mario_falling_forward_sourceRec, mario_falling_forward_destRec, V2n, 0.0f, WHITE);
    }
    else if(!isGrounded && !jump && control == 'l' && play.Position.y+play.Position.height < SCREEN_HEIGHT-78){
        DrawTexturePro(mario_falling_backward, mario_falling_backward_sourceRec, mario_falling_backward_destRec, V2n, 0.0f, WHITE);
    }
    else{
        if(control == 'r'){
            DrawTexturePro(mario_forward, mario_forward_sourceRec, mario_forward_destRec, V2n, 0.0f, WHITE);
        }
        else if (control == 'l'){
            DrawTexturePro(mario_backward, mario_backward_sourceRec, mario_backward_destRec, V2n, 0.0f, WHITE);
        }
        else{
            DrawTexturePro(mario_forward, mario_forward_sourceRec, mario_forward_destRec, V2n, 0.0f, WHITE);

        }
    }

    EndDrawing();
}

void plataform_collision(Rectangle mario_forward_destRec, Rectangle mario_backward_destRec, Map_1 load, Player *play){
    int l = 0;
    for(int i = 0; i<MAX_PLATAFORMS; i++){
        if (CheckCollisionRecs(mario_forward_destRec, load.plataforms[i].Position ))
        {
            isGrounded = true;
            l=i;
        }
    }
    if (CheckCollisionRecs(mario_forward_destRec, load.plataforms[l].Position ))
    {
        isGrounded = true;
    }
    else isGrounded = false;

}

