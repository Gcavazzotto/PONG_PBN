#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

// Tamanho da janela
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

// Tamanho dos paddles e da bola
#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 200
#define BALL_SIZE 20

// Estados do jogo
typedef enum {
    MENU_PRINCIPAL,
    JOGO_PVP,
    JOGO_PVC,
    JOGO_PVCDIficil,
    SAIR
} Estado;

// Função para desenhar texto na tela
void desenhaTexto(SDL_Renderer* renderer, TTF_Font* font, const char* texto, int x, int y, SDL_Color cor) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, texto, cor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstrect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Função para resetar a posição e velocidade da bola
void resetBola(SDL_Rect* bola, int* speedX, int* speedY) {
    bola->x = (WINDOW_WIDTH - BALL_SIZE) / 2;
    bola->y = (WINDOW_HEIGHT - BALL_SIZE) / 2;
    *speedX = (rand() % 2 == 0) ? 8 : -8;
    *speedY = (rand() % 2 == 0) ? 8 : -8;
}

// Jogo jogador vs computador (PvC)
void jogoPvcFacil(SDL_Renderer* renderer, TTF_Font* font, Estado* estado) {
    SDL_Rect paddle1 = {50, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect paddle2 = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect ball = { (WINDOW_WIDTH - BALL_SIZE) / 2, (WINDOW_HEIGHT - BALL_SIZE) / 2, BALL_SIZE, BALL_SIZE };
    int ballSpeedX = 4;
    int ballSpeedY = 4;
    int placar1 = 0, placar2 = 0;
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *estado = SAIR;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    *estado = MENU_PRINCIPAL;
                    quit = true;
                }
            }
        }

        // Movimento do paddle1 (jogador) - setas cima/baixo
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP] && paddle1.y > 0) paddle1.y -= 10;
        if (state[SDL_SCANCODE_DOWN] && paddle1.y + PADDLE_HEIGHT < WINDOW_HEIGHT) paddle1.y += 10;

        // Movimento do paddle2 (IA simples)
        if (ball.y + BALL_SIZE / 2 > paddle2.y + PADDLE_HEIGHT / 2 && paddle2.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            paddle2.y += 6;
        else if (ball.y + BALL_SIZE / 2 < paddle2.y + PADDLE_HEIGHT / 2 && paddle2.y > 0)
            paddle2.y -= 6;

        // Movimento da bola
        ball.x += ballSpeedX;
        ball.y += ballSpeedY;

        // Seta a dificuldade
    

        // Colisão com o topo e fundo da janela
        if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT)
            ballSpeedY = -ballSpeedY;

        // Colisão com os paddles
        if (SDL_HasIntersection(&ball, &paddle1))
            ballSpeedX = -ballSpeedX;
        if (SDL_HasIntersection(&ball, &paddle2))
            ballSpeedX = -ballSpeedX;

        // Verifica pontos
        if (ball.x <= 0) {
            placar2++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        } else if (ball.x + BALL_SIZE >= WINDOW_WIDTH) {
            placar1++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        }

        // Limpa tela (preto)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha paddles e bola (branco)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        SDL_RenderFillRect(renderer, &paddle2);
        SDL_RenderFillRect(renderer, &ball);

        // Desenha placar
        char placarTexto[50];
        sprintf(placarTexto, "Jogador: %d | Computador: %d", placar1, placar2);
        desenhaTexto(renderer, font, placarTexto, (WINDOW_WIDTH/2)-400  , 20, (SDL_Color){255,255,255,255});

        // Instruções para voltar ao menu
        desenhaTexto(renderer, font, "PvC - ESC para voltar", 20, WINDOW_HEIGHT - 40, (SDL_Color){255,255,255,255});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

}
//----------------------------------------------------------------------------------------
void jogoPvcDificil(SDL_Renderer* renderer, TTF_Font* font, Estado* estado) {
    SDL_Rect paddle1 = {50, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect paddle2 = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect ball = { (WINDOW_WIDTH - BALL_SIZE) / 2, (WINDOW_HEIGHT - BALL_SIZE) / 2, BALL_SIZE, BALL_SIZE };
    int ballSpeedX = 10;
    int ballSpeedY = 10;
    int placar1 = 0, placar2 = 0;
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *estado = SAIR;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    *estado = MENU_PRINCIPAL;
                    quit = true;
                }
            }
        }

        // Movimento do paddle1 (jogador) - setas cima/baixo
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP] && paddle1.y > 0) paddle1.y -= 10;
        if (state[SDL_SCANCODE_DOWN] && paddle1.y + PADDLE_HEIGHT < WINDOW_HEIGHT) paddle1.y += 10;

        // Movimento do paddle2 (IA simples)
        if (ball.y + BALL_SIZE / 2 > paddle2.y + PADDLE_HEIGHT / 2 && paddle2.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            paddle2.y += 6;
        else if (ball.y + BALL_SIZE / 2 < paddle2.y + PADDLE_HEIGHT / 2 && paddle2.y > 0)
            paddle2.y -= 6;

        // Movimento da bola
        ball.x += ballSpeedX;
        ball.y += ballSpeedY;

        // Seta a dificuldade
    

        // Colisão com o topo e fundo da janela
        if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT)
            ballSpeedY = -ballSpeedY;

        // Colisão com os paddles
        if (SDL_HasIntersection(&ball, &paddle1))
            ballSpeedX = -ballSpeedX;
        if (SDL_HasIntersection(&ball, &paddle2))
            ballSpeedX = -ballSpeedX;

        // Verifica pontos
        if (ball.x <= 0) {
            placar2++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        } else if (ball.x + BALL_SIZE >= WINDOW_WIDTH) {
            placar1++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        }

        // Limpa tela (preto)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha paddles e bola (branco)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        SDL_RenderFillRect(renderer, &paddle2);
        SDL_RenderFillRect(renderer, &ball);

        // Desenha placar
        char placarTexto[50];
        sprintf(placarTexto, "Jogador: %d | Computador: %d", placar1, placar2);
        desenhaTexto(renderer, font, placarTexto, (WINDOW_WIDTH/2)-400  , 20, (SDL_Color){255,255,255,255});

        // Instruções para voltar ao menu
        desenhaTexto(renderer, font, "PvC - ESC para voltar", 20, WINDOW_HEIGHT - 40, (SDL_Color){255,255,255,255});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------
// Jogo jogador vs jogador (PvP)
void jogoPVP(SDL_Renderer* renderer, TTF_Font* font, Estado* estado) {
    SDL_Rect paddle1 = {50, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect paddle2 = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect ball = { (WINDOW_WIDTH - BALL_SIZE) / 2, (WINDOW_HEIGHT - BALL_SIZE) / 2, BALL_SIZE, BALL_SIZE };
    int ballSpeedX = 40;
    int ballSpeedY = 40;
    int placar1 = 0, placar2 = 0;
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *estado = SAIR;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    *estado = MENU_PRINCIPAL;
                    quit = true;
                }
            }
        }

        // Movimento paddle1 - W e S
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W] && paddle1.y > 0) paddle1.y -= 10;
        if (state[SDL_SCANCODE_S] && paddle1.y + PADDLE_HEIGHT < WINDOW_HEIGHT) paddle1.y += 10;

        // Movimento paddle2 - UP e DOWN
        if (state[SDL_SCANCODE_UP] && paddle2.y > 0) paddle2.y -= 10;
        if (state[SDL_SCANCODE_DOWN] && paddle2.y + PADDLE_HEIGHT < WINDOW_HEIGHT) paddle2.y += 10;

        // Movimento da bola
        ball.x += ballSpeedX;
        ball.y += ballSpeedY;

        // Colisão com topo e fundo da janela
        if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT)
            ballSpeedY = -ballSpeedY;

        // Colisão com paddles
        if (SDL_HasIntersection(&ball, &paddle1))
            ballSpeedX = -ballSpeedX;
        if (SDL_HasIntersection(&ball, &paddle2))
            ballSpeedX = -ballSpeedX;

        // Verifica pontos
        if (ball.x <= 0) {
            placar2++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        } else if (ball.x + BALL_SIZE >= WINDOW_WIDTH) {
            placar1++;
            resetBola(&ball, &ballSpeedX, &ballSpeedY);
        }

        // Limpa tela (preto)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha paddles e bola (branco)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        SDL_RenderFillRect(renderer, &paddle2);
        SDL_RenderFillRect(renderer, &ball);

        // Desenha placar
        char placarTexto[50];
        sprintf(placarTexto, "Jogador 1: %d   |   Jogador 2: %d", placar1, placar2);
        desenhaTexto(renderer, font, placarTexto, (WINDOW_WIDTH / 2) - 180, 20, (SDL_Color){255,255,255,255});

        // Instruções para voltar ao menu
        desenhaTexto(renderer, font, "PvP - ESC para voltar", 20, WINDOW_HEIGHT - 40, (SDL_Color){255,255,255,255});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // Inicializa SDL e TTF
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("Erro TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
        printf("Erro IMG_Init: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Cria janela e renderizador
    SDL_Window* window = SDL_CreateWindow("Pong-Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("Erro SDL_CreateWindow: %s\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Carrega fonte para texto do menu e placar
    TTF_Font* font = TTF_OpenFont("PressStart2P.ttf", 32);
    if (!font) {
        printf("Erro TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Carrega textura do fundo para o menu principal
    SDL_Texture* texturaFundo = IMG_LoadTexture(renderer, "fundo_retro.jpg");
    if (!texturaFundo) {
        printf("Erro carregando fundo_retro.jpg: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Estado estado = MENU_PRINCIPAL;
    SDL_Event event;

    // Opções do menu principal
    int opcaoSelecionada = 0;
    const char* opcoesMenu[] = {
        "1. Jogar PvP",
        "2. Jogar PvC Modo Facil(IA)",
        "3. Jogador PVC Dificil(IA)",
        "4. Sair"
    };
    int numOpcoes = sizeof(opcoesMenu)/sizeof(opcoesMenu[0]);

    // Loop principal do programa
    while (estado != SAIR) {
        switch (estado) {
            case MENU_PRINCIPAL:    
                // Trata eventos no menu
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        estado = SAIR;
                    } else if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_UP) {
                            opcaoSelecionada--;
                            if (opcaoSelecionada < 0) opcaoSelecionada = numOpcoes - 1;
                        } else if (event.key.keysym.sym == SDLK_DOWN) {
                            opcaoSelecionada++;
                            if (opcaoSelecionada >= numOpcoes) opcaoSelecionada = 0;
                        } else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                            if (opcaoSelecionada == 0) estado = JOGO_PVP;
                            else if (opcaoSelecionada == 1) estado = JOGO_PVC;
                            else if (opcaoSelecionada == 2) estado = JOGO_PVCDIficil;
                            else if (opcaoSelecionada == 3) estado = SAIR;
                        }
                    }
                }

                // Limpa tela e desenha fundo
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texturaFundo, NULL, NULL);

                // Desenha título do jogo
                desenhaTexto(renderer, font, "PONG-PONG", WINDOW_WIDTH/2 - 200, 100, (SDL_Color){255,255,255,255});

                // Desenha as opções do menu
                for (int i = 0; i < numOpcoes; i++) {
                    SDL_Color cor = (i == opcaoSelecionada) ? (SDL_Color){238,130,238} : (SDL_Color){255,255,255,255};
                    desenhaTexto(renderer, font, opcoesMenu[i], WINDOW_WIDTH/2 - 300, 400 + i*60, cor);
                }

                SDL_RenderPresent(renderer);
                SDL_Delay(16);
                break;

            case JOGO_PVP:
                jogoPVP(renderer, font, &estado);
                break;

            case JOGO_PVC:
                jogoPvcFacil(renderer, font, &estado);
                break;
            case JOGO_PVCDIficil:
                jogoPvcDificil(renderer, font, &estado);
                break;
                

            default:
                estado = SAIR;
                break;
        }
    }

    // Limpeza final
    SDL_DestroyTexture(texturaFundo);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
