#include "verkko.h"
#include "display.h"
#include "solu.h"

#include <iostream>
#include <sstream>
#include <time.h>
#include <thread>
#include <string>
#include <fstream>

using namespace std;

void wait() {
	char c;
	cin >> c;
}


int STEPS = 0;

enum ui_T{
	UI_QUIT = -1, UI_DEFAULT = 0, UI_RESET = 1
};


struct kuuntelija{
	ui_T viesti;
	void kuuntele();
	void aloita();
};


void kuuntelija::kuuntele() {
	while (viesti != UI_QUIT){
		string s;
		stringstream ss;
		int arg;

		//viesti = UI_DEFAULT;

		cout << ">";

		getline(cin, s);

		ss << s;
		ss >> s;
		ss >> arg;
		static int savedcount = 0;

		if (s.compare("limit") == 0) {
			if (arg > 0 && arg < solu::settings.maxValue) {
				solu::settings.limit = arg;
				cout << "Set limit " << arg << "\n";
			}
		}
		else if (s.compare("life") == 0) {
			if (arg > 0 && arg < solu::settings.maxValue) {
				solu::settings.life = arg;
				cout << "Set Life change " << arg << "\n";
			}
		}
		else if (s.compare("death") == 0) {
			if (arg > 0 && arg < solu::settings.maxValue) {
				solu::settings.death = arg;
				cout << "Set Death change " << arg << "\n";
			}
		}
		else if (s.compare("lifepercent") == 0) {
			solu::settings.lifePercent = arg;
			cout << "Set Life percent " << arg << "\n";
		}
		else if (s.compare("zoom") == 0) {
			if (arg == 0) {
				setDisplayZoom(0);
				cout << "Zoom pois\n";
			}
			else if (arg == 1) {
				setDisplayZoom(1);
				cout << "Zoom päällä\n";
			}
			else
				cout << "Huono parametri " << arg << "\n";
			
		}
		else if (s.compare("save") == 0) {
			stringstream iss;
			string is;
			iss << savedcount;
			iss >> is;
			string filename = "solu" + is;
			tallennaKuva(filename);
			cout << "Tallennettiin " << filename << "\n";
			savedcount++;
		}
		else if (s.compare("savesettings") == 0)
			solu::settings.save();
		else if (s.compare("loadsettings") == 0)
			solu::settings.load();
		else if (s.compare("time") == 0 || s.compare("aika") == 0) {
			kerroAjat();
			cout << "Steps: " << STEPS << "\n";
		}
		else if (s.compare("randomize") == 0 || s.compare("reset")==0 )   //reset = randomize+clear
			solu::settings.randomizeRule();

		if (s.compare("quit") == 0 || s.compare("lopeta") == 0 || s.compare("exit") == 0)
			viesti = UI_QUIT;
		else if (s.compare("reset") == 0 || s.compare("clear") == 0) {
			viesti = UI_RESET;
			STEPS = 0;
		}
		else
			viesti = UI_DEFAULT;
	}
}


void kuuntelija::aloita() {
	thread(&kuuntelija::kuuntele, this).detach();
}


int main(int argc, char* argv[]) {
	
	int verkko_w = 800;
	int verkko_h = 800;
	int ruutu_w = 800;
	int ruutu_h = 800;

	if (argc == 5) {
		ruutu_w = atoi(argv[3]);
		ruutu_h = atoi(argv[4]);
	}
	if (argc >= 3) {
		verkko_w = atoi(argv[1]);
		verkko_h = atoi(argv[2]);
	}

	initVerkko(verkko_w, verkko_h, ruutu_w, ruutu_h);

	cout << sizeof(solu);

	kuuntelija k;
	k.aloita();

	while (k.viesti != UI_QUIT) {
		laskeVerkko();
		piirraVerkko();

		if (k.viesti == UI_RESET) {
			resetVerkko();
			k.viesti = UI_DEFAULT;
		}

		STEPS++;
	}

	suljeVerkko();
	return 0;

}