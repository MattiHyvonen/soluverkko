#ifndef SOLU_H
#define SOLU_H

#include <array>
#include <vector>
#include "settings.h"

class solu{
protected:
	short A;
	short B;
	char elo; //tilat: 0, 1, 2, 3  <=>  0; A; B; A&B
	
	//status on A_summa + 9*B_summa
	char status;

	solu** naapurit = NULL;

public:

	static settingsT settings;

	solu();
	void kytke(std::vector<solu*> kohteet);
	void asetaArvot(short a, short b);

	void laskeStatus();	//tuota status naapurien elon perusteella
	void laskeArvot();	//päivitä arvot statuksen perusteella
	void laskeElo();	//päivitä elo arvojen perusteella

	//palauta eloarvo muuttamatta sitä
	bool onkoElossa_A();
	bool onkoElossa_B();

	//palauta arvo muuttamatta sitä
	short haeArvo_A();		
	short haeArvo_B();
};

#endif