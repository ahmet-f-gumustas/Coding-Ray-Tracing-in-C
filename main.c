#include <stdio.h>
#include <SDL2/SDL.h>
#include "raytracer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void render_scene(SDL_Renderer* renderer, Sphere* spheres, int sphere_count) {
    Camera camera = {
        .position = vec3(0, 0, 0),
        .at = vec3(0, 0, -1),
        .up = vec3(0, 1, 0),
        .fov = 90
    };
    
    double aspect_ratio = (double)SCREEN_WIDTH / SCREEN_HEIGHT;
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;
    
    Vec3 horizontal = vec3(viewport_width, 0, 0);
    Vec3 vertical = vec3(0, viewport_height, 0);
    Vec3 lower_left_corner = vec3_sub(
        vec3_sub(vec3_sub(camera.position, vec3_div(horizontal, 2)), 
                 vec3_div(vertical, 2)), 
        vec3(0, 0, focal_length)
    );
    
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            double u = (double)x / (SCREEN_WIDTH - 1);
            double v = (double)(SCREEN_HEIGHT - 1 - y) / (SCREEN_HEIGHT - 1);
            
            Vec3 direction = vec3_sub(
                vec3_add(
                    vec3_add(lower_left_corner, vec3_mul(horizontal, u)),
                    vec3_mul(vertical, v)
                ),
                camera.position
            );
            
            Ray r = ray(camera.position, direction);
            Vec3 color = ray_color(r, spheres, sphere_count);
            
            int r_val = (int)(255.99 * color.x);
            int g_val = (int)(255.99 * color.y);
            int b_val = (int)(255.99 * color.z);
            
            SDL_SetRenderDrawColor(renderer, r_val, g_val, b_val, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow(
        "Ray Tracer",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    Sphere spheres[] = {
        {vec3(0, 0, -1), 0.5, vec3(1.0, 0.0, 0.0)},
        {vec3(-1, 0, -1), 0.5, vec3(0.0, 1.0, 0.0)},
        {vec3(1, 0, -1), 0.5, vec3(0.0, 0.0, 1.0)},
        {vec3(0, -100.5, -1), 100, vec3(0.8, 0.8, 0.0)}
    };
    int sphere_count = sizeof(spheres) / sizeof(spheres[0]);
    
    SDL_Event e;
    bool quit = false;
    bool rendered = false;
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        if (!rendered) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            printf("Rendering scene... This may take a moment.\n");
            render_scene(renderer, spheres, sphere_count);
            
            SDL_RenderPresent(renderer);
            rendered = true;
            printf("Rendering complete!\n");
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}