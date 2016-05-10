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

	int P_A = rand() % 41 + 60;		// P(A!=0) [%]
	int P_Apos = rand() % 41;	    // P(A==1), kun A!=0 [%]
	int P_B = P_A;		// jne
	int P_Bpos = P_Apos;	

	for (int i = 0; i < 256; i++) {
		/*
		char A = 0;
		char B = 0;
		if (rand() % 100 < P_A) {
			if (rand() % 100 < P_Apos)
				A = 1;
			else A = -1;
		}
		if (rand() % 100 < P_B) {
			if (rand() % 100 < P_Bpos)
				B = 1;
			else B = -1;
		}
		*/

		A_rule[i] = rand() % (2*life) - life;
		B_rule[i] = rand() % (2 * life) - life;
	}
}


void settingsT::setRule(Uint32* a, Uint32* b) {
	for (int i = 0; i < 256; i++) {
		A_rule[i] = a[i] - 8; 
		B_rule[i] = b[i] - 8;
	}
}


/*get, set & add*/
void settingsT::load() {
	if (!list.empty())
	*this = list[index]; 
}

void settingsT::save() { 
	if (list.empty())
		add();
	else
		list[index] = *this;
}

void settingsT::add() { 
	list.push_back(*this); 
}

void settingsT::printOut() {
	/*std::cout << "A:";
	for (int i = 0; i < 81; i++) {
		if (i % 9 == 0) std::cout << "\n";
		if (A_rule[i] == -1) std::cout << 'D';
		else if (A_rule[i] == 0) std::cout << '0';
		else if (A_rule[i] == 1) std::cout << 'L';
	}

	std::cout << "\nB:";

	for (int i = 0; i < 81; i++) {
		if (i % 9 == 0) std::cout << "\n";
		if (B_rule[i] == -1) std::cout << 'D';
		else if (B_rule[i] == 0) std::cout << '0';
		else if (B_rule[i] == 1) std::cout << 'L';
	}
	std::cout << '\n';*/
}