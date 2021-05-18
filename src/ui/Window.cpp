#include <iostream>

#include "../core/Utils.h"

#include "Window.h"

UI::Window::Window(const std::string &windowTitle, const std::shared_ptr<Keyboard> &keyboard) {
    if (Core::Utils::debugL2()) {
        std::cout << "Window construct" << std::endl;
    }

    this->windowTitle = windowTitle;
    this->keyboard = keyboard;

    this->closed = true;
    this->window = nullptr;
    this->renderer = nullptr;
    this->font = nullptr;
}

UI::Window::~Window() {
    if (Core::Utils::debugL2()) {
        std::cout << "Window destruct" << std::endl;
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool UI::Window::show() {
    if (init()) {
        closed = false;
        return true;
    }

    return false;
}

bool UI::Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Window: SDL Init video failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Window: SDL TTF failed: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
            windowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT,
            SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );

    if (window == nullptr) {
        std::cerr << "Window: SDL Window failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        std::cerr << "Window: SDL Renderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("fonts/Hack-Regular.ttf", FONT_SIZE);

    if (!font) {
        std::cerr << "Window: SDL TTF load font failed: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // Fix scaling on retina displays

    return true;
}

void UI::Window::redraw() {
    SDL_RenderPresent(renderer);
}

void UI::Window::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void UI::Window::pollEvents() {
    SDL_Event windowEvent;

    if (SDL_WaitEvent(&windowEvent)) {
        if (windowEvent.type == SDL_QUIT) {
            closed = true;
        } else if (windowEvent.type == SDL_KEYUP) {
            keyboard->keyUp(windowEvent.key.keysym.sym);
        }
    }
}

bool UI::Window::isClosed() const {
    return closed;
}

void UI::Window::drawText(const std::string &text, const int xPosition, const int yPosition) {
    SDL_Surface *surface = TTF_RenderText_Shaded(font, text.c_str(), ORANGE, BLACK);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destination{xPosition, yPosition, surface->w, surface->h};

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &destination);
    SDL_DestroyTexture(texture);
}
