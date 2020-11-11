//
// Created by uberdever on 06.09.2020.
//

#include "game.h"

int main() {
    alphabeta_init((gamma_info_t){800, 600, false, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL, 0});

    const uint frameskip = 1000 / 60;
    uint time = SDL_GetTicks();

    events_set_handler(event_handler);
    init();
    while (is_program_running) {
        if (time + frameskip <= SDL_GetTicks())
        {
            time = SDL_GetTicks();
            begin_frame();
            events_process();
            update();
            render();
            end_frame();
        }
    }
    clean();
    alphabeta_clean();
    return 0;
}