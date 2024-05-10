#include <SDL.h>
#include <iostream>

struct Object {
    float x, y;//위치
    float vx, vy;//속도

    Object(float x, float y) : x(x), y(y), vx(0.0f), vy(0.0f) {}//생성자
};

struct item {
    float x, y;//위치
    item(float x, float y) : x(x), y(y){}//생성자
};


const int screen_x = 800, screen_y = 600;

const int radius = 10;

const float start_x = screen_x / 2, start_y = screen_y - 30;

const float gravity = 0.098f;//중력 상수.

const float jump_speed = -3.5f;//초기 y축 속도.

int over = 0, score = 0;

int main(int argc, char* argv[]) {
    SDL_Window* window = SDL_CreateWindow("gravity", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_x, screen_y, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Object obj(start_x, start_y);
    Object redObj(start_x - 50, start_y); 
    Object whiteObj(start_x + 50, start_y - 20); 
    Object choco(0, start_y);
    item item1(start_x + 240, start_y -80);
    item item2(start_x, start_y - 80);

    bool g_flag_running = false;
    bool jumping_1 = false;
    bool jumping_2 = false;

    //이벤트 처리 루프
    while (!g_flag_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                g_flag_running = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP && !jumping_1) {
                    obj.vy = jump_speed;
                    jumping_1 = true;
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    obj.vx = -1.0f;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    obj.vx = 1.0f;
                }
                else if (event.key.keysym.sym == SDLK_a) {
                    redObj.vx = -1.0f;
                }
                else if (event.key.keysym.sym == SDLK_d) {
                    redObj.vx = 1.0f;
                }
                else if (event.key.keysym.sym == SDLK_w && !jumping_2) {
                    redObj.vy = jump_speed;
                    jumping_2 = true;
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                    obj.vx = 0.0f;
                }
                else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d) {
                    redObj.vx = 0.0f;
                }
            }
        }
        obj.vy += gravity;//초록공 중력 적용.
        redObj.vy += gravity;//빨강공 중력 적용.

        //속도에 따른 초록공 공간 이동.
        if (obj.vx < 0 and obj.x > 10) {
            obj.x += obj.vx;
        }
        else if (obj.vx > 0 and obj.x < screen_x - 10) {
            obj.x += obj.vx;
        }
        obj.y += obj.vy;

        //속도에 따른 빨강공 공간 이동.
        if (redObj.vx < 0 and redObj.x > 10) {
            redObj.x += redObj.vx;
        }
        else if (redObj.vx > 0 and redObj.x < screen_x - 10) {
            redObj.x += redObj.vx;
        }
        redObj.y += redObj.vy;

        //초콜릿대포 이동
        if (choco.x >= 0 and choco.x < screen_x - 10) {
            choco.x += 2;
            if (choco.x == screen_x - 10) {
                choco.x=0;
            }
        }

        //상자(밀릴 물체) 공간 이동.
        if (whiteObj.vx < 0 and whiteObj.x > 10) {
            whiteObj.x += whiteObj.vx;
            whiteObj.vx = 0;
        }
        else if (whiteObj.vx > 0 and whiteObj.x < screen_x - 10) {
            whiteObj.x += whiteObj.vx;
            whiteObj.vx = 0;
        }

        //플레이어의 점프.
        if (obj.y >= start_y) {
            obj.y = start_y;
            obj.vy = 0.0f;
            jumping_1 = false;
        }
        if (redObj.y >= start_y) {
            redObj.y = start_y;
            redObj.vy = 0.0f;
            jumping_2 = false;
        }

        //붉은 공이 상자와 왼쪽에서 충돌.
        if (redObj.x== whiteObj.x-radius && redObj.y - radius > whiteObj.y) {
            redObj.x = whiteObj.x - radius - 1;
            whiteObj.vx = 0.0f;
        }

        //붉은 공이 상자와 오른쪽에서 충돌.
        else if (redObj.x - radius == whiteObj.x + 29 && redObj.y - radius > whiteObj.y) {
            redObj.x = whiteObj.x + radius + 30;
            whiteObj.vx = 0.0f;
        }

        //붉은 공이 상자 위에 안착.
        else if ((redObj.x > whiteObj.x - radius and redObj.x < whiteObj.x + 29 + radius) and redObj.y + radius > whiteObj.y) {
            redObj.y = whiteObj.y - 11;
            redObj.vy = 0.0f;
            jumping_2 = false;
        }

        //초록 공이 상자와 충돌해서 욺김.
        if ((obj.x + radius == whiteObj.x or obj.x - radius == whiteObj.x + 29) && obj.y - radius > whiteObj.y) {
            whiteObj.vx = obj.vx;
        }

        //초록 공이 상자 위에 안착.
        else if ((obj.x > whiteObj.x - radius and obj.x < whiteObj.x + 29 + radius) and obj.y + radius > whiteObj.y) {
            obj.y = whiteObj.y - 11;
            obj.vy = 0.0f;
            jumping_1 = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect groundRect = { 0, screen_y - 20, screen_x, 20 };
        SDL_RenderFillRect(renderer, &groundRect);

        //공 제작.
        SDL_SetRenderDrawColor(renderer, 50, 150, 10, 255);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(obj.x) + dx, static_cast<int>(obj.y) + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(redObj.x) + dx, static_cast<int>(redObj.y) + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 127, 82, 0, 255);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(choco.x) + dx, static_cast<int>(choco.y) + dy);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,100, 100, 40, 255);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(item1.x) + dx, static_cast<int>(item1.y) + dy);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 200, 100, 40, 255);
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(item2.x) + dx, static_cast<int>(item2.y) + dy);
                }
            }
        }

        //상자 제작.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect whiteRect = { static_cast<int>(whiteObj.x), static_cast<int>(whiteObj.y), 30, 30 };

        //충촐검사를 위해 SDL_Rect
        SDL_Rect objRect = { static_cast<int>(obj.x - radius), static_cast<int>(obj.y - radius), radius * 2, radius * 2 };
        SDL_Rect redRect = { static_cast<int>(redObj.x - radius), static_cast<int>(redObj.y - radius), radius * 2, radius * 2 };
        SDL_Rect chocoRect = { static_cast<int>(choco.x - radius), static_cast<int>(choco.y - radius), radius * 2, radius * 2 };
        SDL_Rect item1Rect = { static_cast<int>(item1.x - radius), static_cast<int>(item1.y - radius), radius * 2, radius * 2 };
        SDL_Rect item2Rect = { static_cast<int>(item2.x - radius), static_cast<int>(item2.y - radius), radius * 2, radius * 2 };

        if (SDL_HasIntersection(&objRect, &chocoRect)) {
            //충돌이 체크됐으면 화면밖으로 버림
            obj.x = -2000;
            obj.y = -2000; 
            obj.vx = 0.0f; 
            obj.vy = 0.0f; 
            over++;
        }
        if (SDL_HasIntersection(&redRect, &chocoRect)) {
            //충돌이 체크됐으면 화면밖으로 버림
            redObj.x = -2000; 
            redObj.y = -2000; 
            redObj.vx = 0.0f;
            redObj.vy = 0.0f; 
            over++;
        }
        if (over == 2) {
            break;
        }

        if (SDL_HasIntersection(&redRect, &item1Rect)) {
            //충돌이 체크됐으면 점수를 먹고 화면밖으로 버림
            score+=10;
            item1.x = -2000;
            item1.y = -2000;
        }
        if (SDL_HasIntersection(&objRect, &item2Rect)) {
            score+=10;
            item2.x = -2000;
            item2.y = -2000;
        }

        std::cout << score << "\n";
        SDL_RenderFillRect(renderer, &whiteRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
