#include "solu.h"
#include <bitset>
#include <array>
#include <iostream>

using namespace std;

settingsT solu::settings;


solu::solu() {
	A = 0;
	B = 0;
	laskeElo();
}


void solu::kytke(std::vector<solu*> kohteet) {
	if (kohteet.size() < 8) {
		cerr << "Huonot kohteet\n";
		return;
	}

	if (naapurit == NULL)
		naapurit = new solu*[8];

	for (int i = 0; i < 8; i++)
		naapurit[i] = kohteet[i];
}


void solu::asetaArvot(short a, short b) {
	A = a;
	B = b;
	laskeElo();
}


void solu::laskeStatus(){	//tuota status naapurien elon perusteella                    
	status = 0;
	if (naapurit[0]->onkoElossa_A()) status += 1;
	if (naapurit[0]->onkoElossa_B()) status += 9;
	if (naapurit[1]->onkoElossa_A()) status += 1;
	if (naapurit[1]->onkoElossa_B()) status += 9;
	if (naapurit[2]->onkoElossa_A()) status += 1;
	if (naapurit[2]->onkoElossa_B()) status += 9;
	if (naapurit[3]->onkoElossa_A()) status += 1;
	if (naapurit[3]->onkoElossa_B()) status += 9;
	if (naapurit[4]->onkoElossa_A()) status += 1;
	if (naapurit[4]->onkoElossa_B()) status += 9;
	if (naapurit[5]->onkoElossa_A()) status += 1;
	if (naapurit[5]->onkoElossa_B()) status += 9;
	if (naapurit[6]->onkoElossa_A()) status += 1;
	if (naapurit[6]->onkoElossa_B()) status += 9;
	if (naapurit[7]->onkoElossa_A()) status += 1;
	if (naapurit[7]->onkoElossa_B()) status += 9;

}


void solu::laskeArvot(){	//päivitä arvo statuksen perusteella
	if (settings.A_rule[status] > 0)
		A += settings.life;
	else if(settings.A_rule[status] < 0)
		A -= settings.death;
	if (A < 0) A = 0;
	if (A > settings.maxValue) A = settings.maxValue;
	
	
	if (settings.B_rule[status] > 0)
		B += settings.life;
	else if (settings.B_rule[status] < 0)
		B -= settings.death;
	if (B < 0) B = 0;
	if (B > settings.maxValue) B = settings.maxValue;

}


void solu::laskeElo(){	//päivitä elo arvojen perusteella
	elo = 0;
	if (A >= settings.limit) elo |= 1;
	if (B >= settings.limit) elo |= 2;
}


bool solu::onkoElossa_A() { return (elo & 1); }
bool solu::onkoElossa_B() { return (elo & 2); }

short solu::haeArvo_A() { return A; }
short solu::haeArvo_B() { return B; }

