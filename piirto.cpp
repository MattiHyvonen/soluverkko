#include "piirto.h"
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <string>

using namespace std;


bool initDisplay() {
	static bool ready = false;

	if (!ready) {
		srand(time(NULL));

		/*initialize sdl for video*/
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			cerr << "Init error: " << SDL_GetError() << '\n';
			ready = false;
		}

		else {
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
				cerr << "IMG_init error\n";
				ready = false;
			}
			else {
				ready = true;
				cout << "Init succesful\n";
			}
		}

	}
	return ready;
}


SDL_Rect makeRect(int x, int y, int w = 0, int h = 0) {
	SDL_Rect result;
	result.x = x;
	result.y = y;
	result.w = w;
	result.h = h;
	return result;
}


bool isInside(int x, int y, SDL_Rect& rect) {
	if (x < rect.x) return false;
	else if (y < rect.y) return false;
	else if (x > rect.x + rect.w) return false;
	else if (y > rect.y + rect.h) return false;
	else return true;
}


Uint32 makePixel_rgba8888(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 pixel = (r << 24) | (g << 16) | (b << 8) | a;
	return pixel;
}


bool ikkuna::init(int position_x, int position_y) {
	
	if (!ready) {

		cout << "Init window called\n";

		if (!initDisplay()) {
			cout << "initDisplay failed! " << SDL_GetError() << "\n";
			return false;
		}

		window = SDL_CreateWindow(title.c_str(), position_x, position_y, w, h, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			cout << "Couldn't create window! " << SDL_GetError() << "\n";
			return false;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == NULL) {
			cout << "Couldn't create renderer! " << SDL_GetError() << "\n";
			return false;
		}

		cout << "Init window succesful!\n";

		ready = true;
	}
	return ready;
}

ikkuna::ikkuna(int width, int height, std::string _title) : w(width), h(height), title(_title), ready(false) {
}


void ikkuna::show() {
	SDL_RenderPresent(renderer);
}


image::image() : w(0), h(0), texture(NULL), target(NULL) , place(makeRect(0,0,0,0) ) {}

void image::create(int _w, int _h, ikkuna* _target) {	create(0, 0, _w, _h, _target);}

void image::create(int _x, int _y, int _w, int _h, ikkuna* _target) {

	w = _w;
	h = _h;
	target = _target;
	place = makeRect(_x, _y, _w, _h);

	if (target == NULL)
		cout << "Bad target!\n";
	else
		texture = SDL_CreateTexture(target->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}


void image::createFromFile(std::string path, ikkuna* _target) {
	
	target = _target;
	
	SDL_Surface* loaded = IMG_Load(path.c_str() );
	if (loaded == NULL)
		std::cout << "Couldn't load image " << path << "! " << IMG_GetError() << "\n";
	else if (target == NULL)
		cout << "Bad target!\n";
	else if (target->renderer == NULL)
		std::cout << "Bad renderer!\n";
	else {
		texture = SDL_CreateTextureFromSurface(target->renderer, loaded);

		if (texture == NULL)
			std::cout << "Couldn't create texture from image! " << SDL_GetError() << "\n";

		w = loaded->w;
		h = loaded->h;

		place = makeRect(0, 0, w, h);

		SDL_FreeSurface(loaded);
	}
}


void image::show(int _x, int _y, int _w, int _h) {
	SDL_Rect dst = makeRect(_x, _y, _w, _h);
	SDL_RenderCopy(target->renderer, texture, NULL, &dst);
}

void image::show() { 
	SDL_RenderCopy(target->renderer, texture, NULL, &place);
}


bool interfaceWindow::init() {
	bool result = ikkuna::init(100,100);
	
	if (result == false)
		cout << "Init failed\n";
	
	else{
		//create buttons
		buttons.resize(6);

		for (int i = 0; i < 6; i++) {
			buttons[i].createFromFile("D:\\button.png", this);
			buttons[i].place = makeRect(580, i * (h / 6) + 10, 200, 80);
		}

		//create field
		ruleField.create(16, 16, this, 16);
		ruleField.place = makeRect(20,20,540,540);
		//ruleField.randomize();
		ruleField.update();

		result = true;
	}
	return result;
}


void interfaceWindow::show() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].show();
	}

	ruleField.update();
	ruleField.show();

	SDL_RenderPresent(renderer);
}


soluEventT interfaceWindow::handleEvent() {
	SDL_Event event;
	soluEventT result = NONE;
	Uint32 windowID = SDL_GetWindowID(window);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
			cout << "Quit!\n";
			return QUIT;
		}

		else if (event.window.windowID == windowID && event.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			for (int i = 0; i < buttons.size(); i++) {
				if (isInside(x, y, buttons[i].place)) {
					cout << "button " << i << " pressed!\n"; //do that button's function!
					if (i == 0) return CLEAR;
					else if (i == 1) return CLEAR_RULE;
				}

				if (isInside(x, y, ruleField.place)) {
					result = CHANGED;
					int relative_x = x - ruleField.place.x;
					int relative_y = y - ruleField.place.y;

					//hae se solu jonka päällä hiiri on (16x16 ruudukosta)
					int col = 16 * (float)relative_x / ruleField.place.w;
					int row = 16 * (float)relative_y / ruleField.place.h;
					cout << "Solu " << col << ":" << row << "\n";

					int d = 0;

					if (event.button.button == SDL_BUTTON_LEFT) d = 1;
					else if (event.button.button == SDL_BUTTON_RIGHT) d = -1;

					ruleField.changePixel(col, row, d);

				}
			}
		}
	}
	return result;
}


Uint32* interfaceWindow::getRule() {
	return ruleField.getValues();
}

bool soluWindow::init(int solut_w, int solut_h) {
	bool result = ikkuna::init(1000, 100);

	if (result == false)
		cout << "Init failed\n";
	else{
		solut.create(solut_w, solut_h, this, 65536);

		result = true;
	}
	return result;
}


void soluWindow::show() {
	solut.update();
	solut.show(0,0,w,h);

	SDL_RenderPresent(renderer);
}


bool palettedField::init() {
	
	size = w*h;
	
	pixels = new Uint32[size];
	values = new Uint32[size];
	palette = new Uint32[paletteSize];

	//setup palette
	Uint8 red;
	Uint8 green;
	Uint8 blue;

	int pitch = sqrt(paletteSize);

	for (int j = 0; j < pitch; j ++) {

		green = ((float)j / pitch) * 256;
		blue = green;

		for (int i = 0; i < pitch; i++) {
			//grayscale:
			red = ((float)i / pitch) * 256;

			palette[i + j*pitch] = makePixel_rgba8888(red, green, blue, 255);
		}
	}

	clear();

	return true;
}


void palettedField::clear() {
	for (int i = 0; i < size; i++)
		putPixel(i, 8);
}

void palettedField::create(int width, int height, ikkuna* _target, int palSize) {
	
	paletteSize = palSize;
	image::create(width, height, _target);

	init();
}


void palettedField::putPixel(int i, Uint32 value) {
	if (value < paletteSize && i>=0 && i<size)
		values[i] = value;
}


void palettedField::putPixel(int x, int y, Uint32 value) {
	putPixel(y*w + x, value);
}


void palettedField::changePixel(int i, int value) {
	int newValue = values[i] + value;
	if (newValue < 0) values[i] = 0;
	else if (newValue >= paletteSize) values[i] = paletteSize - 1;
	else values[i] = newValue;
}

void palettedField::changePixel(int x, int y, int value) {
	changePixel(y*w + x, value);
}


Uint32 palettedField::getPixel(int i) {
	return values[i];
}

Uint32 palettedField::getPixel(int x, int y, int value) {
	return getPixel(y*w + x);
}

Uint32* palettedField::getValues() {
	return values;
}

void palettedField::update() {

	for (int i = 0; i < size; i++)
		pixels[i] = palette[values[i]];

	SDL_UpdateTexture(texture, NULL, pixels, w*sizeof(Uint32));
}


void palettedField::randomize() {
	for (int i = 0; i < size; i++)
		putPixel(i, rand()%paletteSize);
}