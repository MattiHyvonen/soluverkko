#ifndef VERKKO_H
#define VERKKO_H

#include "solu.h"

#include <array>
#include <string>

void initVerkko(int verkonLeveys = 100, int verkonKorkeus = 100, int ruudunLeveys = 600, int ruudunKorkeus = 500, int paletinKoko = 765);
void resetVerkko();
void laskeVerkko();
void suljeVerkko();
void piirraVerkko();


void kerroAjat();
void tallennaKuva(std::string);

/*settings
void tallennaAsetukset();
void lataaAsetukset();

void setLife(int life);
void setDeath(int death);
void setLimit(int limit);
void setLifePercent(int lifePercent);
void randomizeRule(int lifePercent);
void randomizeRule();
*/

class avg{
protected:
	double sum = 0;
	int count = 0;
public:
	void put(double a) {
		sum += a;
		count++;
	}

	double get() {
		return sum / count;
	}
};

#endif