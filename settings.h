#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

/*load from & save to conf file*/
void loadSettings();
void saveSettings();

/*selecting*/
void select(int index_);
void next();
void random();

class settingsT{
public:
	int life;
	int death;
	int limit;
	int maxValue;
	int lifePercent;
	std::array<int, 9> rule;

	settingsT() : life(20), death(30), limit(30), maxValue(765), lifePercent(-1) {}

	void randomizeRule();

	/*load from list, save to list, add to list*/
	void load();
	void save();
	void add();

};


#endif