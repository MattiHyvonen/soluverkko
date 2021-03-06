#include "display.h"
#include "verkko.h"

#include <iostream>
#include <time.h>
#include <thread>
#include <fstream>
#include <vector>

using namespace std;



//--------------GLOBAALIT MUUTTUJAT HUOM -----------------//


//const string CONF_FILE = "settings.conf";

int W;
int H;
int SIZE;
int PALSIZE;

//vector<solu> solut;
solu* solut;
pixelField* piirto;

avg statusKello;
avg arvoKello;
avg piirtoKello;
clock_t initAika;

//--------------GLOBAALIT MUUTTUJAT HUOM -----------------//

/*
void lataaAsetukset() {  //lataa s��nn�n tiedostosta

	ifstream file(CONF_FILE, ios::in | ios::binary | ios::ate);
	soluSettingsT* s = new soluSettingsT;

	//lue tiedostosta settings-objekti
	if (file.is_open()) {
		if (file.tellg() >= sizeof(soluSettingsT)) {
			file.seekg(0, ios::beg);
			file.read((char*)s, sizeof(soluSettingsT));
			file.close();
		}
	}

	//aseta solujen asetukset
	solu::settings = *s;
	
	delete s;
}


void tallennaAsetukset() {
	ofstream file(CONF_FILE, ios::out | ios::binary | ios::ate);

	//kirjoita tiedostoon
	if (file.is_open()) {
		file.seekp(0, ios::beg);
		file.write((char*) &solu::settings, sizeof(soluSettingsT));
		file.close();
	}
}



void setLife(int life) { solu::settings.life = life;  }
void setDeath(int death) { solu::settings.death = death; }
void setLimit(int limit) { solu::settings.limit = limit; }
void setLifePercent(int lifePercent) { solu::settings.lifePercent = lifePercent; }

void randomizeRule(int lifePercent) {
	
	if (lifePercent <= 0 || lifePercent > 100)
		lifePercent = rand() % 70 + 15;

	for (int i = 0; i < 9; i++) {
		solu::settings.rule[i] = (rand() % 3 - 1);
	}

	//solu::settings.rule[0] = 0;
	//solu::settings.rule[255] = 0;
}

void randomizeRule() {
	randomizeRule(solu::settings.lifePercent);
}
*/


void kytkeVerkko() {	//kytke kaikki solut toisiinsa
	
	//huom t�t� voi optimoida paljonkin
	int prev_x, prev_y, next_x, next_y;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			if (x == 0) prev_x = W - 1;
			else prev_x = x - 1;
			if (x == W - 1) next_x = 0;
			else next_x = x + 1;
			if (y == 0) prev_y = H - 1;
			else prev_y = y - 1;
			if (y == H - 1) next_y = 0;
			else next_y = y + 1;

			vector<solu*> naapurit;
			naapurit.push_back(&solut[y*W + prev_x]);		//vasen
			naapurit.push_back(&solut[prev_y*W + prev_x]);	//yl�vasen
			naapurit.push_back(&solut[prev_y*W + x]);		//yl�
			naapurit.push_back(&solut[prev_y*W + next_x]);	//yl�oikea
			naapurit.push_back(&solut[y*W + next_x]);		//oikea
			naapurit.push_back(&solut[next_y*W + next_x]);	//alaoikea
			naapurit.push_back(&solut[next_y*W + x]);		//ala
			naapurit.push_back(&solut[next_y*W + prev_x]);	//alavasen

			solut[y*W + x].kytke(naapurit);
		}
	}

}


void initVerkko(int verkonLeveys, int verkonKorkeus, int ruudunLeveys, int ruudunKorkeus, int paletinKoko) {
	
	srand(time(NULL));

	clock_t t = clock();
	
	W = verkonLeveys;
	H = verkonKorkeus;
	SIZE = W*H;
	PALSIZE = paletinKoko;

	initDisplay(ruudunLeveys, ruudunKorkeus);

	piirto = new pixelField;
	piirto->initialize(W, H, PALSIZE);

	solut = new solu[SIZE]; //t�ss� menee KAUAN
	
	kytkeVerkko();
	
	//lataaAsetukset();//lataa rule tiedostosta
	
	loadSettings(); //lataa tallennetut asetukset muistiin
	solu::settings.load(); // aseta solun asetukset
	
	resetVerkko();

	initAika = clock() - t;

}


void laskeStatus_osa(int alku, int loppu) {
	for (int i = alku; i < loppu; i++)
		solut[i].laskeStatus();
}


void laskeStatus() {
	vector<thread> threads;
	int nThreads = std::thread::hardware_concurrency();
	for (int i = 0; i < nThreads; i++) {

		int alku = i * (SIZE / nThreads);
		int loppu = alku + (SIZE / nThreads);

		threads.push_back(thread(laskeStatus_osa, alku, loppu));
	}

	for (int i = 0; i < nThreads; i++) {
		threads[i].join();
	}
}


void laskeArvot_osa(int alku, int loppu) {
	for (int i = alku; i < loppu; i++) {
		solut[i].laskeArvo();
		solut[i].laskeElo();
	}
}


void laskeArvot() {
	vector<thread> threads;
	int nThreads = std::thread::hardware_concurrency();
	for (int i = 0; i < nThreads; i++) {

		int alku = i * (SIZE / nThreads);
		int loppu = alku + (SIZE / nThreads);

		threads.push_back(thread(laskeArvot_osa, alku, loppu));
	}

	for (int i = 0; i < nThreads; i++) {
		threads[i].join();
	}
}


void clearVerkko() {
	for (int i = 0; i < SIZE; i++) {
		solut[i].asetaArvo(0);
	}
}


void luoSoluja(){
	//lis�t��n soluja
	for (int t = 0; t < 300; t++){
		int x = (W / 2) + (rand() % 41 - 20);
		int y = (H / 2) + (rand() % 41 - 20);

		int i = y*W + x;
		solut[i].asetaArvo(rand() % 256);
	}

}


void resetVerkko() {
	clearVerkko();
	luoSoluja();
}


void laskeVerkko(){
	clock_t t = clock();
	
	//Laske status
	laskeStatus();
	statusKello.put(clock() - t);

	//laske arvot
	t = clock();
	laskeArvot();
	arvoKello.put(clock() - t);
}


void suljeVerkko() {

	piirto->close();
	closeDisplay();

	delete[] solut; //t�ss� menee KAUAN
}


void piirraVerkko() {
	clock_t t = clock();
	for (int y = 0; y < H; y++){
		for (int x = 0; x < W; x++) {
			piirto->putPixel(x, y, solut[y*W + x].haeArvo());
		}
	}

	piirto->show();
	piirtoKello.put(clock() - t);
}


void kerroAjat() {
	cout << "Laskuajat:\n";
	cout << "Init " << initAika << " ms\n";
	cout << "Status " << statusKello.get() << " ms\n";
	cout << "Arvo " << arvoKello.get() << " ms\n";
	cout << "Piirto " << piirtoKello.get() << " ms\n";
}


void tallennaKuva(string filename) {

	string fullfile = "D:\ " + filename + ".png";

	piirto->save(fullfile);
}