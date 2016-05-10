#ifndef PIIRTO_H
#define PIIRTO_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

const int IKKUNA_DEFAULT_W = 640;
const int IKKUNA_DEFAULT_H = 480;
const int DEFAULT_PALSIZE = 256;
const std::string IKKUNA_DEFAULT_TITLE = "Cellular";

class ikkuna;
class interfaceWindow;
class soluWindow;
class image;
class palettedField;


/*SDL window that can draw textures*/
class ikkuna {
public:
	SDL_Renderer* renderer = NULL;
	SDL_Window* window = NULL;

	int w;
	int h;
	bool ready;
	std::string title;

public:
	ikkuna(int width = IKKUNA_DEFAULT_W, int height = IKKUNA_DEFAULT_H, std::string title = IKKUNA_DEFAULT_TITLE);
	bool init(int position_x = SDL_WINDOWPOS_CENTERED, int position_y = SDL_WINDOWPOS_CENTERED);
	void show();
};


class image{
public:
	SDL_Texture* texture;
	int w;
	int h;
	ikkuna* target;
	SDL_Rect place;

	image();

	void create(int w, int h, ikkuna* target);
	void create(int x, int y, int w, int h, ikkuna* target);
	void createFromFile(std::string path, ikkuna* target);
	void show();
	void show(int x, int y, int w, int h);

};


class palettedField : public image {
protected:
	int size;
	int paletteSize;

	Uint32* palette = NULL;
	Uint32* pixels = NULL;
	Uint32* values = NULL;

	bool init();

public:

	void create(int w, int h, ikkuna* target, int palSize = DEFAULT_PALSIZE);

	void putPixel(int i, Uint32 value);
	void putPixel(int x, int y, Uint32 value);
	
	void changePixel(int i, int value);
	void changePixel(int x, int y, int value);

	Uint32 getPixel(int i);
	Uint32 getPixel(int x, int y, int value);

	Uint32* getValues();

	void update();

	void randomize();
	void clear();
};


enum soluEventT{
	NONE = 0, QUIT = -1, CLEAR = 2, CHANGED = 3, CLEAR_RULE = 4
};


class interfaceWindow : public ikkuna{
public:
	std::vector<image> buttons;
	palettedField ruleField;

	interfaceWindow(int width = 800, int height = 800) : ikkuna(width, height) {}
	bool init();
	void show();
	Uint32* getRule();

	soluEventT handleEvent();
};


class soluWindow : public ikkuna{
public:

	palettedField solut;

	soluWindow(int width = 800, int height = 800) : ikkuna(width, height) {}
	bool init(int solut_w, int solut_h);
	void show();

};

#endif