#ifndef INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H
#define INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Keyboard.h"

namespace UI {

    /**
     * Manages a window with support for drawing text.
     */
    class Window {

    public:
        Window(const std::string &windowTitle, const std::shared_ptr<Keyboard> &keyboard);
        ~Window();

        bool show();
        void redraw();
        void clearScreen();
        void pollEvents();
        [[nodiscard]] bool isClosed() const;
        void drawText(const std::string &text, int xPosition, int yPosition);

    private:
        static const int WIDTH = 800;
        static const int HEIGHT = 400;
        static const int FONT_SIZE = 18;

        static constexpr SDL_Color ORANGE = {255, 165, 0};
        static constexpr SDL_Color BLACK = {0, 0, 0};

        std::shared_ptr<Keyboard> keyboard;
        std::string windowTitle;
        bool closed;

        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;

        bool init();
    };
}

#endif //INC_8_BIT_COMPUTER_EMULATOR_WINDOW_H
