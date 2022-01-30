#include <SDL2/SDL.h>
#include <random>
#define LOGICAL_WINDOW_WIDTH 128
#define ACTUAL_WINDOW_WIDTH 1024

//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

// something needs to bedone about this  
// Use density, and type (static or dynamic)
enum pixel_state
{
    empty,
    // gas,
	fluid,
    solid,
    // fire,
    // burning,
    // burnt,
    fixed_pos
};
struct position
{
    pixel_state state_now;
	// Yes I could do all these vars in one line, but this is "cleaner"
    uint8_t  r,g,b,a;
	uint8_t density; 
	uint8_t pressure;
    uint16_t temperature;
};

struct cord_2d
{
    int x_pos;
    int y_pos;
};

typedef std::chrono::high_resolution_clock Clock;
