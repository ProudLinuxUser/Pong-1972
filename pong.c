#include <time.h>
#include <raylib.h>
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int main(void){
        int WIDTH = 800;
        int HEIGHT = 600;
        InitWindow(WIDTH,HEIGHT,"Pong");
        Image icon = LoadImage("pong.png");
        SetWindowIcon(icon);
        UnloadImage(icon);
        SetTargetFPS(60);

        //random
        int options[] = {1,-1};
        srand(time(NULL));
        int n = sizeof(options)/sizeof(options[0]);
        int random = options[rand() % n];

        //Ball
        int BALLX       = 400;
        int BALLY       = 300;
        int ball_speedX = 5;
        int ball_speedY = 5;

        //Points
        int PLAYER_POINT = 0;
        int BOT_POINT    = 0;
        float SPEED        = 5;
        float BOT_SPEED    = 4.5;

        //Player && Bot rect
        int Px = 60;
        int Py = 40;
        int Bx = 730;
        int By = 40;
        bool game_on = false;
        bool restart = false;
        Rectangle player = {Px,Py,15,100};
        Rectangle bot    = {Bx,By,15,100};

        //Background
        Rectangle bg     = {0,0,800,600};
        GuiSetStyle(DEFAULT,TEXT_SIZE,30);
        GuiSetStyle(DEFAULT,BASE_COLOR_NORMAL,ColorToInt(WHITE));

        while (!WindowShouldClose()){

                if (restart){
                        game_on = false;
                        PLAYER_POINT = 0;
                        BOT_POINT = 0;
                }

                if (!game_on){
                        //Ball moving
                        restart = false;
                        BALLX += ball_speedX;
                        BALLY += ball_speedY;

                        //Player movement
                        if (IsKeyDown(KEY_UP))   player.y -= SPEED;
                        if (IsKeyDown(KEY_DOWN)) player.y += SPEED;

                        //Bot movement
                        if (BALLY > bot.y + bot.height){
                                bot.y += BOT_SPEED;
                        }
                        if (BALLY < bot.y){
                                bot.y -= BOT_SPEED;
                        }

                        //Out of bounds check
                        if (player.y >= 500)  player.y = 500;
                        if (player.y <= 0)    player.y = 0;
                        if (bot.y >= 500)        bot.y = 500;
                        if (bot.y <= 0)          bot.y = 0;

                        //Ball collision
                        if (BALLX - 20 <= 0||  BALLX - 780 >= 0) ball_speedX = -ball_speedX;
                        if (BALLY - 20 <= 0 || BALLY - 580 >= 0) ball_speedY = -ball_speedY;

                        //Point check
                        if (BALLX - 20 == 0){
                                BOT_POINT++;
                                BALLX = WIDTH/2;
                        }
                        if (BALLX - 780 == 0){
                                PLAYER_POINT++;
                                BALLX = WIDTH/2;
                        }

                        //Ball collision
                        if (CheckCollisionCircleRec((Vector2){BALLX,BALLY}, 15, player)){
                                ball_speedX = 5;
                                ball_speedY = 5 * (rand() % 2 == 0 ? 1 : -1);
                                BALLX = player.x + player.width + 15;
                        }
                        if (CheckCollisionCircleRec((Vector2){BALLX,BALLY}, 15, bot)){
                                ball_speedX = -5;
                                ball_speedY = 5 * (rand() %2 == 0 ? 1 : -1);
                                BALLX = bot.x - 15;
                        }
                }

                //Drawing a bit of style
                for (int y = 0 ; y != 600 ; y += 25){
                           DrawCircle(397,y,3,WHITE);
                }

                //Reset && quit options
                if (PLAYER_POINT == 15){
                        DrawText("Player won!",250,270,50,WHITE);
                        game_on = true;
                        if (GuiButton((Rectangle){250,325,300,100},"Restart")){
                                restart = true;
                        }
                        if (GuiButton((Rectangle){250,425,300,100},"Exit")){
                                CloseWindow();
                        }

                }else if (BOT_POINT == 15){
                        DrawText("Bot won!",300,250,50,WHITE);
                        game_on = true;
                        if (GuiButton((Rectangle){250,325,300,100},"Restart")){
                                restart = true;
                        }
                        if (GuiButton((Rectangle){250,425,300,100},"Exit")){
                                CloseWindow();
                        }
                }

                BeginDrawing();
                //reseting canva every frame
                ClearBackground(BLACK);

                //Drawing ball
                DrawRectangle(BALLX,BALLY,20,20,WHITE);

                //Drawing points
                DrawText(TextFormat("%.2d",PLAYER_POINT),235,10,100,WHITE);
                DrawText(TextFormat("%.2d",BOT_POINT),450,10,100,WHITE);

                //map outline,player,bot
                DrawRectangleLinesEx(bg,10,WHITE);
                DrawRectangleRec(player,WHITE);
                DrawRectangleRec(bot,WHITE);
                EndDrawing();
        }
        CloseWindow();
}
