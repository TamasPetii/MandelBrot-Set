#include <SDL.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

double asd = 1.0;

double scale_pixel(double x, int w_min, int w_max, double scale_min, double scale_max) {
	return (x - w_min) * (scale_max - scale_min) / (w_max - w_min) + scale_min;
}

void fractal(SDL_Renderer* render, int x, int y, double px, double py) {

	long double x0 = scale_pixel(x, WIDTH - WIDTH * asd, WIDTH * asd, -2.0, 0.48);
	long double y0 = scale_pixel(y, HEIGHT - HEIGHT * asd, HEIGHT * asd, -1.12, 1.12);

	int i = 0;
	while ((px*px + py*py) <= 4 && i < 100) {
		long double pxtemp = px * px - py * py + x0;
		py = 2 * px * py + y0;
		px = pxtemp;
		i++;
	}
	if (i != 100) {
		SDL_SetRenderDrawColor(render, 255 * ((i % 10) / 10.0), 255 * ((i % 10) / 10.0), 255 * ((i % 10) / 10.0), 255);
		SDL_RenderDrawPoint(render, x, y);
	}
}

void drawFractal(SDL_Renderer* render, double px, double py) {
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			fractal(render, x, y, px, py);
		}
	}
	SDL_RenderPresent(render);
}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("MandelBrot Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH , HEIGHT, 2);
	if (window == nullptr) { std::cout << "Cannot create window!" << std::endl; SDL_Quit();  exit(1); }

	SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
	if (render == nullptr) { std::cout << "Cannot create renderer" << std::endl; SDL_DestroyWindow(window); SDL_Quit();  exit(1); }
	
	SDL_RenderPresent(render);
	drawFractal(render, 0, 0);
	SDL_RenderPresent(render);

	SDL_Event event;
	bool quit = false;
	double px = 0;
	double py = 0;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_q) {
					asd -= 0.05;
					drawFractal(render, 0, 0);
				}
				if (event.key.keysym.sym == SDLK_w) {
					asd += 0.05;
					drawFractal(render, 0, 0);
				}
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);
	SDL_Quit();
	return 0;
}
