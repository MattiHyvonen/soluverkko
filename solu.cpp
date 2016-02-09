#include "solu.h"
#include <bitset>
#include <array>
#include <iostream>

using namespace std;

settingsT solu::settings;


solu::solu() {
	arvo = 0;
	laskeElo();
}


void solu::kytke(std::vector<solu*> kohteet) {
	if (naapurit == NULL)
		naapurit = new solu*[8];
	
	for (int i = 0; i < 8; i++)
		naapurit[i] = kohteet[i];
	//naapurit = kohteet;
}


void solu::asetaArvo(int a) {
	arvo = a;
	laskeElo();
}


void solu::laskeStatus(){	//tuota status naapurien elon perusteella                    
	status = 0;
	status += naapurit[0]->onkoElossa();
	status += naapurit[1]->onkoElossa();
	status += naapurit[2]->onkoElossa();
	status += naapurit[3]->onkoElossa();
	status += naapurit[4]->onkoElossa();
	status += naapurit[5]->onkoElossa();
	status += naapurit[6]->onkoElossa();
	status += naapurit[7]->onkoElossa();
}


int solu::laskeArvo(){	//päivitä arvo statuksen perusteella
	if (settings.rule[status] > 0)
		arvo+= settings.life;
	else if(settings.rule[status] < 0)
		arvo-= settings.death;

	if (arvo < 0) arvo = 0;
	if (arvo > settings.maxValue) arvo = settings.maxValue;

	return arvo;
}


bool solu::laskeElo(){	//päivitä elo arvon perusteella
	elo = (arvo >= settings.limit);
	return elo;
}


bool solu::onkoElossa() { return elo; }


int solu::haeArvo() { return arvo; }

