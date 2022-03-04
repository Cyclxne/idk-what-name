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
#include "ui/stb_truetype.h"

#include "init_funcs.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "input_large.hpp"
#include "rand_testing.hpp"
#include "generation/terain.hpp"
#include "debug/printing.hpp"
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

    colour colour_to_change;
    colour_to_change.r = 115;
    colour_to_change.g = 85;
    colour_to_change.b = 104;
    colour_to_change.a = 255;

    colour colour_to_change2;
    colour_to_change.r = 46;
    colour_to_change.g = 253;
    colour_to_change.b = 124;
    colour_to_change.a = 255;

    // draw_box_white_sand(box_verts[0],box_verts[1], pixels);
    // ui::draw_box(box_verts[0],box_verts[1], colour_to_change, user_interface);

    std::cout << "To make the box" << '\n';
    // ui::boxes boxy_1(box_verts[0], box_verts[1], colour_to_change);
    // std::shared_ptr<ui::boxes> ptr_to_boxy(new ui::boxes(box_verts[0], box_verts[1], colour_to_change));
    // ui_elements.push_back(new ui::boxes(box_verts[0], box_verts[1], colour_to_change);
    // ui::storeObject(std::make_shared<ui::single_ui_element>(ui::boxes(box_verts[0], box_verts[1], colour_to_change)), ui_elements);
    // std::cout << "To make the text" << '\n';
    // std::shared_ptr<ui::text> ptr_to_texty(new ui::text(box_verts[0], box_verts[1], colour_to_change2, "Hello world", "ui/font/Hack-Regular.ttf"));

    ui_elements.push_back(new ui::boxes(box_verts[0], box_verts[1], colour_to_change));
    ui_elements.push_back(new ui::text(box_verts[0], box_verts[1], colour_to_change2, " Hello", "ui/font/Hack-Regular.ttf", 15));


    std::cout << "Done ui" << '\n';

    // bool quit = false;
    std::thread physics(Physics::simulate, pixels, new_version);
    // int frame_count = 0;
    while (!quit_now)
    {
        // auto start_time = Clock::now();
        // Debug_Printing::print_density(pixels);
        // std::cout << '\n';

        mtx2.lock();
        quit_now = Input_Large::poll_usr_input(changed, usr_input, &event, quit_now, actual_2_logic_ratio);
        Input_Large::mix_new_version_usr_input(changed, usr_input, pixels);
        std::memcpy(&render, &pixels, sizeof(pixels));
        mtx2.unlock();

        // std::cout << "Made it?" << '\n';
        // ui_elements[0]->draw(render);

        // ui::boxes *box_prt = dynamic_cast<ui::boxes *>(ui_elements[0]);
        // box_prt->draw(render);
        ui_elements[0]->draw(render);
        ui_elements[1]->draw(render);
        // std::cout << "Made it" << '\n';

        // boxy_1.draw(render);
        Render::redraw_render(render, renderer);
        SDL_RenderPresent(renderer);

        // auto end_time = Clock::now();
    }
    // Debug_Printing::print_density(pixels);
    physics.join();
    std::cout << "ENDED" << '\n';
    // std::cout << "Sand: " << Testing::count_sand(pixels) << '\n';
    excecution_finished();
}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
