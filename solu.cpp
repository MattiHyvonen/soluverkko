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
	/*status = 0;
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

	// 0...80
	*/
	
	int asum = 0;
	int bsum = 0;

	asum += naapurit[0]->haeArvo_A();
	bsum += naapurit[0]->haeArvo_B();
	asum += naapurit[1]->haeArvo_A();
	bsum += naapurit[1]->haeArvo_B();
	asum += naapurit[2]->haeArvo_A();
	bsum += naapurit[2]->haeArvo_B();
	asum += naapurit[3]->haeArvo_A();
	bsum += naapurit[3]->haeArvo_B();
	asum += naapurit[4]->haeArvo_A();
	bsum += naapurit[4]->haeArvo_B();
	asum += naapurit[5]->haeArvo_A();
	bsum += naapurit[5]->haeArvo_B();
	asum += naapurit[6]->haeArvo_A();
	bsum += naapurit[6]->haeArvo_B();
	asum += naapurit[7]->haeArvo_A();
	bsum += naapurit[7]->haeArvo_B();

	//0...8*255

	asum /= 128; //oli 144 
	bsum /= 128;

	//0...15

	status = asum + bsum * 16;

	//0...255
	
	if (status < 0) status = 0;
	else if (status > 255) status = 255;
}


void solu::laskeArvot(){	//päivitä arvo statuksen perusteella
	/*if (settings.A_rule[status] > 0)
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
	if (B > settings.maxValue) B = settings.maxValue;*/

	A += settings.A_rule[status];
	if (A < 0) A = 0;
	if (A > settings.maxValue) A = settings.maxValue;

	B += settings.B_rule[status];
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

