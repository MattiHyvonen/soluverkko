#ifndef SOLU_H
#define SOLU_H

#include <array>
#include <vector>
#include "settings.h"

class solu{
protected:
	short arvo;
	char elo;
	char status;		//naapurien elossaolon perusteella koodattu luku.
	solu** naapurit = NULL;

public:

	static settingsT settings;

	solu();
	void kytke(std::vector<solu*> kohteet);
	void asetaArvo(int a);

	void laskeStatus();	//tuota status naapurien elon perusteella
	int laskeArvo();	//p�ivit� arvo statuksen perusteella
	bool laskeElo();	//p�ivit� elo arvon perusteella

	bool onkoElossa();	//palauta eloarvo muuttamatta sit�
	int haeArvo();		//palauta arvo muuttamatta sit�

};

#endif