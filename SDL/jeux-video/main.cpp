#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <memory>
#define STB_TRUETYPE_IMPLEMENTATION
#define BLEND_TEXT 0
#include "ui/stb_truetype.h"
#include "math/vector_math.hpp"
#include "init_funcs.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "input_large.hpp"
#include "rand_testing.hpp"
#include "generation/terain.hpp"
// #include "debug/printing.hpp"
#include "ui/ui.hpp"

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

// std::vector<std::unique_ptr<ui::boxes>> ui_elements;
std::vector<ui::single_ui_element *> ui_elements;

std::mutex mtx2;

int main()
{
    std::cout << "Starting" << '\n';
    init::start();
    std::cout << "Started, allocating memory" << '\n';

    init::array_clean_start(pixels);
    const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;
    std::cout << "Thingy started now" << '\n';
    Terrain::Development Terrain_gen;
    Terrain_gen.Development::generate_terrain(0, LOGICAL_WINDOW_WIDTH / 2, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH, 8, pixels);

    cord_2d box_verts[2];
    box_verts[0].x_pos = 0;
    box_verts[1].x_pos = 100;
    box_verts[0].y_pos = 0;
    box_verts[1].y_pos = 25;

    cord_2d text_position;
    text_position.x_pos = 0;
    text_position.y_pos = 5;
    std::cout << "Txt pos " << text_position.x_pos << " " <<  text_position.y_pos << '\n';

    colour colour_to_change;
    colour_to_change.r = 231;
    colour_to_change.g = 63;
    colour_to_change.b = 0;
    colour_to_change.a = 255;

    colour colour_to_change2;
    colour_to_change2.r = 46;
    colour_to_change2.g = 253;
    colour_to_change2.b = 124;
    colour_to_change2.a = 255;

    ui::init_font_all("ui/font/Hack-Regular.ttf");

    ui_elements.push_back(new ui::boxes(box_verts[0], box_verts[1], colour_to_change));
    ui_elements.push_back(new ui::text(text_position, colour_to_change2, " Jackie", 40, 0));

    std::cout << "Done ui" << '\n';

    // bool quit = false;
    std::thread physics(Physics::simulate, pixels, new_version);
    // int frame_count = 0;
    while (!quit_now)
    {

        mtx2.lock();
        quit_now = Input_Large::poll_usr_input(changed, usr_input, &event, quit_now, actual_2_logic_ratio);
        Input_Large::mix_new_version_usr_input(changed, usr_input, pixels);
        std::memcpy(&render, &pixels, sizeof(pixels));
        mtx2.unlock();

        ui_elements[0]->draw(render);
        ui_elements[1]->draw(render);

        Render::redraw_render(render, renderer);
        SDL_RenderPresent(renderer);
    }
    physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();
}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
