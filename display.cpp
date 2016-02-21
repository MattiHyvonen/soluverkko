#include "display.h"

#include <iostream>

using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

int gWidth;
int gHeight;
bool zoom = true;

Uint32 makePixel_RGBA8888(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 pixel = (r << 24) | (g << 16) | (b << 8) | a;
	return pixel;
}


bool initDisplay(int w, int h, bool fullscreen) {  //luo ikkuna ja aloita piirtäminen
	static bool ready = false;

	if (ready)
		return true;

	else{
		if (SDL_Init(SDL_INIT_VIDEO) < 0){
			return false;
		}

		gWidth = w;
		gHeight = h;

		//piilota hiiren osoitin
		SDL_ShowCursor(0);

		//luo ikkuna keskelle
		int fsFlags;
		if (fullscreen)
			fsFlags = SDL_WINDOW_FULLSCREEN_DESKTOP;
		else
			fsFlags = SDL_WINDOW_SHOWN;

		gWindow = SDL_CreateWindow("Soluverkko", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, fsFlags);
		if (gWindow == NULL)
			return false;

		//luo renderer
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL)
			return false;

		//luo piirtoikkunan tekstuuri
		gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		//jos tähän päästiin, kaikki onnistui.
		ready = true;
		return true;
	}
}


void closeDisplay(){  //sulje piirtoikkuna
	if(gTexture != NULL) 
		SDL_DestroyTexture(gTexture);

	if (gRenderer != NULL)
		SDL_DestroyRenderer(gRenderer);

	if (gWindow != NULL)
		SDL_DestroyWindow(gWindow);

	SDL_Quit();
}


void setDisplayZoom(bool z) { zoom = z; }


void pixelField::setupPalette() {
	int red, green, blue;
	for (int b = 0; b < palSize; b++){
		
		blue = b;

		for (int a = 0; a < palSize; a++) {
			red = a;
			
			green = (red + blue)/2;

			palette[a + palSize*b] = makePixel_RGBA8888(red, green, blue, 255);
		}
	}
	//palette[0] = makePixel_RGBA8888(0,0,255, 255);
}


bool pixelField::initialize(int width, int height, int paletteSize) {  //määritä paletti ja pixels sekä luo tekstuuri

	//estetään jos on jo kutsuttu
	if (ready)
		return true;

	if (width*height <= 0)
		return false;

	if (paletteSize <= 0)
		return false;

	palSize = paletteSize;
	w = width;
	h = height;

	//varaa muisti pikselidatalle ja paletille
	pixels = new Uint32[w*h];
	palette = new Uint32[palSize*palSize];

	//luo tekstuuri
	texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (texture == NULL)
		return false;

	//onnistui jos tähän päästiin
	ready = true;

	//luo vielä paletti
	setupPalette();

	return true; 
}


void pixelField::close() {  //tuhoa tekstuuri, paletti ja pikselit
	
	delete[] pixels;
	delete[] palette;
	
	if (texture != NULL)
		SDL_DestroyTexture(texture);
}


void pixelField::putPixel(int x, int y, Uint32 value) {
	//rajatarkastus. Huom: tämä hidastaa vähän
	if (y < h && x < w) {

		int palSizeSquared = palSize*palSize;

		if (value < 0) 
			value = 0;
		if (value >= palSizeSquared) 
			value = palSizeSquared - 1;

		pixels[y*w + x] = palette[value];
	}
}


void pixelField::putPixel(int x, int y, Uint32 A, Uint32 B) {
	if (A > palSize) A = palSize;
	if (A<0) A = 0;
	if (B > palSize) B = palSize;
	if (B < 0) B = 0;
	putPixel(x, y, A + palSize*B);
}


bool pixelField::show() {   //tee muutokset tekstuuriin ja piirrä ruudulle
	
	if (!ready)
		return false;

	//päivitä pixelFieldin oma tekstuuri pikseleistä
	if (SDL_UpdateTexture(texture, NULL, pixels, w*sizeof(Uint32)) < 0)
		return false;

	SDL_Rect* p_r = NULL;
	SDL_Rect srcRect;

	if (!zoom) {
		srcRect.x = (w-gWidth) / 2;
		srcRect.y = (h-gHeight) / 2;
		srcRect.w = gWidth;
		srcRect.h = gHeight;
		p_r = &srcRect;
	}

	//piirrä pixelFieldin tekstuuri gTextureen
	SDL_SetRenderTarget(gRenderer, gTexture);
	if (SDL_RenderCopy(gRenderer, texture, p_r, NULL) < 0){
		//palauta renderTarget jos epäonnistui
		SDL_SetRenderTarget(gRenderer, NULL);
		return false;
	}

	SDL_RenderPresent(gRenderer);

	//palauta gRendererin kohde oletukseen eli ikkunaan
	SDL_SetRenderTarget(gRenderer, NULL);

	//piirrä gTexture ruudulle. Huom: tämän voisi ulkoistaa pixelFieldistä!
	if (SDL_RenderCopy(gRenderer, gTexture, NULL, NULL) < 0)
		return false;

	SDL_RenderPresent(gRenderer);

	//kerro guille että ohjelma on käynnissä
	SDL_PollEvent(NULL);

	return true;
}


void pixelField::randomize() {
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			putPixel(x, y, (x * y) % palSize);
		}
	}
}


void pixelField::save(string filename) {

	const int rmask = 0xff000000;
	const int gmask = 0x00ff0000;
	const int bmask = 0x0000ff00;
	const int amask = 0x000000ff;

	SDL_Surface* tmp = SDL_CreateRGBSurfaceFrom(pixels, w, h, 32, w*4, rmask, gmask, bmask, amask);
	IMG_SavePNG(tmp, filename.c_str() );
	SDL_FreeSurface(tmp);
}