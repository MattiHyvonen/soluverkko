#include "settings.h"

const char* CONF_FILE = "settings.conf";

std::vector<settingsT> list; //tänne tallennetaan
int index =0;


/*load list from conf file*/
void loadSettings() {

	list.clear();

	std::ifstream file(CONF_FILE, std::ios::in | std::ios::binary | std::ios::ate);
	//lue tiedosto
	if (file.is_open()) {
		size_t size = file.tellg();
		int n = size / sizeof(settingsT);
		if (n > 0) {
			file.seekg(0, std::ios::beg);
			
			//tallenna listaan objektit
			for (int i = 0; i < n; i++){
				char mem[sizeof(settingsT)];
				file.read(mem, sizeof(settingsT));
				settingsT S = *((settingsT*)mem);
				list.push_back(S);
			}

			std::cout << "Luettiin asetukset\n";
			file.close();
		}
	}
	else{
		std::cout << "Ei voitu lukea tiedostoa " << CONF_FILE << "\n";
	}
}

/*save list to conf file*/
void saveSettings() {
	std::ofstream file(CONF_FILE, std::ios::out | std::ios::binary | std::ios::ate);

	//kirjoita tiedostoon listasta
	if (file.is_open()) {
		file.seekp(0, std::ios::beg);
		int n = list.size();

		file.write((char*)&list[0], sizeof(settingsT)*n);

		file.close();
	}
}

/*changing index*/
void select(int index_) {
	index = index_;
	if (index >= list.size()) index = 0;
}

void next() {
	index++;
	if (index >= list.size()) index = 0;
}

void random() {
	index = rand() % list.size();
}

void settingsT::randomizeRule() {
	for (int i = 0; i < 81; i++) {
		A_rule[i] = (rand() % 3 - 1);
		B_rule[i] = (rand() % 3 - 1);
	}
}


/*get, set & add*/
void settingsT::load() {
	if (!list.empty())
	*this = list[index]; 
}
void settingsT::save() { list[index] = *this; }
void settingsT::add() { list.push_back(*this); }
