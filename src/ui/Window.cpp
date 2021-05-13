#include <iostream>

#include "../core/Utils.h"

#include "Window.h"

UI::Window::Window(const std::string &windowTitle) {
    if (Core::Utils::debugL2()) {
        std::cout << "Window construct" << std::endl;
    }

    this->windowTitle = windowTitle;
    this->closed = true;
    this->window = nullptr;
    this->renderer = nullptr;
}

UI::Window::~Window() {
    if (Core::Utils::debugL2()) {
        std::cout << "Window destruct" << std::endl;
    }

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

    if (SDL_PollEvent(&windowEvent)) {
        if (windowEvent.type == SDL_QUIT) {
            closed = true;
        }
    }
}

bool UI::Window::isClosed() const {
    return closed;
}
