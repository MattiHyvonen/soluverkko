#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

const int DISPLAY_DEFAULT_W = 500;
const int DISPLAY_DEFAULT_H = 500;

const int PF_DEFAULT_PALSIZE = 256;
const int PF_DEFAULT_W = 100;
const int PF_DEFAULT_H = 100;


Uint32 makePixel_RGBA8888(Uint8 r, Uint8 g, Uint8 b, Uint8 a);   //luo pikseli rgb-arvoista

//luo ikkuna ja aloita piirt‰minen:
bool initDisplay(int w = DISPLAY_DEFAULT_W, int h = DISPLAY_DEFAULT_H, bool fullscreen = false);

void closeDisplay();	//sulje piirtoikkuna

void setDisplayZoom(bool);


class pixelField{
protected:

	bool ready = false;	//voidaanko piirt‰‰ eli onko initialize() kutsuttu onnistuneesti

	int w, h;		//kent‰n koko
	int palSize;	//paletin koko - montako v‰ri‰ per ulottuvuus

	Uint32* pixels = NULL;
	Uint32* palette = NULL;

	SDL_Texture* texture = NULL;	//konkreettinen tekstuuri

	void setupPalette();	//luo paletti

public:

	//m‰‰rit‰ paletti ja pixels sek‰ luo tekstuuri:
	bool initialize(int width = PF_DEFAULT_W, int height = PF_DEFAULT_H, int paletteSize = PF_DEFAULT_PALSIZE);

	void putPixel(int x, int y, Uint32 value);		//aseta pikseli. Kutsu update t‰m‰n j‰lkeen
	void putPixel(int x, int y, Uint32 A, Uint32 B);

	bool show();	//tee muutokset tekstuuriin ja piirr‰ ruudulle

	void close();	//tuhoa tekstuuri

	void randomize();	//testausta varten

	void save(std::string filename);

};


#endif