#ifndef INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H
#define INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H

#include <memory>
#include <string>

#include <SDL2/SDL.h>

namespace UI {

    class Window {

    public:
        explicit Window(const std::string &windowTitle);
        ~Window();

        bool show();
        void redraw();
        void clearScreen();
        void pollEvents();
        [[nodiscard]] bool isClosed() const;

    private:
        static const int WIDTH = 800;
        static const int HEIGHT = 400;

        std::string windowTitle;
        bool closed;

        SDL_Window *window;
        SDL_Renderer *renderer;

        bool init();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H
