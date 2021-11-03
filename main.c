#include "raylib.h"


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700


typedef struct {
    Vector2 Position;
    Vector2 Size;

    int Ammo;
    int Lives;
    int Points;
} Player;

void init(Player*);
void update(Player*);
void draw(Player, Texture2D , Rectangle, Texture2D, Rectangle, Texture2D, Rectangle, Texture2D, Rectangle);

int main(void) {
    Player play;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu jogador");

    // Mario walking forward
    Texture2D mario_forward = LoadTexture("images/forward.png");
    Rectangle frameRec_forward = { 0.0f, 0.0f, (float)mario_forward.width/4, (float)mario_forward.height };
    int currentFrame_forward = 0;
    int framesCounter_forward = 0;

    // Mario walking backward
    Texture2D mario_backward = LoadTexture("images/backward.png");
    Rectangle frameRec_backward = { 0.0f, 0.0f, (float)mario_backward.width/4, (float)mario_backward.height };
    int currentFrame_backward = 0;
    int framesCounter_backward = 0;

    // Mario jumping forward
    Texture2D mario_jumping_forward = LoadTexture("images/jumping_forward.png");
    Rectangle frameRec_jumping_forward = { 1.0f, 1.0f, (float)mario_jumping_forward.width/1, (float)mario_jumping_forward.height };
    int currentFrame_jumping = 0;
    int framesCounter_jumping = 0;

    // Mario jumping backward
    Texture2D mario_jumping_backward = LoadTexture("images/jumping_backward.png");
    Rectangle frameRec_jumping_backward = { 0.0f, 0.0f, (float)mario_jumping_backward.width/1, (float)mario_jumping_backward.height };

    // Speed ​​at which Mario's movement frames are switched
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
        draw(play, mario_forward, frameRec_forward, mario_backward, frameRec_backward, mario_jumping_forward, frameRec_jumping_forward,
             mario_jumping_backward, frameRec_jumping_backward);

    }

    CloseWindow();

    return 0;
}

// Function that determines Mario startup
void init (Player *play) {

    play->Position = (Vector2) {
        (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2
    };
    play->Size = (Vector2) {
        50, 50
    };
}

// Function that sets Mario's scroll speed
void update (Player *play) {
    if (IsKeyDown(KEY_RIGHT) && play->Position.x+play->Size.x < SCREEN_WIDTH) {
        play->Position.x += 4.0f;
    }
    if (IsKeyDown(KEY_LEFT) && play->Position.x >= 0) {
        play->Position.x -= 4.0f;
    }
    if (IsKeyDown(KEY_UP) && play->Position.y >= 0) {
        play->Position.y -= 4.0f;
    }
    if (IsKeyDown(KEY_DOWN) && play->Position.y+play->Size.y < SCREEN_HEIGHT) {
        play->Position.y += 4.0f;
    }
}

void draw (Player play, Texture2D mario_forward, Rectangle frameRec_forward, Texture2D mario_backward,
           Rectangle frameRec_backward, Texture2D mario_jumping_forward, Rectangle frameRec_jumping_forward,
           Texture2D mario_jumping_backward, Rectangle frameRec_jumping_backward) {

    BeginDrawing();

    ClearBackground(RAYWHITE);

    char control;

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
