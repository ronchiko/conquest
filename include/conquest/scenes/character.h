#pragma once

#include <vector>

#include "conquest/graphics/screen.h"
#include "conquest/weapons.h"

#define MAX_NAME_LENGTH		 32
#define MAX_RACE_NAME_LENGTH 16

#define STAT_MAX (STAT_LUCK + 1)

// The race of character, defines changes to the base stats & defines the growth rate of each stat
class Race : public IName
{
public:
	static std::vector<Race *> all;
	static void load(void);

	Race(const char *name);

	const char *getName(void) const override
	{
		return name;
	}

	void setStatModifer(Stats stat, int mod)
	{
		statTable[stat] = mod;
	}

	void setGrowthRate(Stats stat, float mod)
	{
		growthTable[stat] = mod;
	}

	int getStatModifer(Stats stat) const
	{
		return statTable[stat];
	}

	float getGrowthRate(Stats stat) const
	{
		return growthTable[stat];
	}

private:
	char name[MAX_RACE_NAME_LENGTH + 1];
	int statTable[STAT_MAX];
	float growthTable[STAT_MAX];
};

class Proffesion : public IName
{
public:
	static std::vector<Proffesion *> all;
	static void load(void);

	Proffesion(const char *name);

	const char *getName(void) const override
	{
		return name;
	}

	void setStatModifer(Stats stat, int mod)
	{
		statTable[stat] = mod;
	}

	int getStatModifer(Stats stat) const
	{
		return statTable[stat];
	}

private:
	char name[MAX_RACE_NAME_LENGTH + 1];
	int statTable[STAT_MAX];
};

class Character : public IName
{
public:
	Character(const char *name);

	const char *getName(void) const override
	{
		return name;
	}

	void setProffesion(Proffesion *p)
	{
		proffesion = p;
	}

	void setRace(Race *r)
	{
		race = r;
	}

	void setElement(Element *elem)
	{
		element = elem;
	}

	const Element *getElement(void) const
	{
		return element;
	}

	const Race *getRace(void) const
	{
		return race;
	}

	const Proffesion *getProffesion(void) const
	{
		return proffesion;
	}

private:
	int level = 1;
	char name[MAX_NAME_LENGTH + 1];
	Race *race;
	Proffesion *proffesion;
	Element *element;
	std::vector<Trait *> traits;
	int stats[STAT_MAX];
};

template<class T>
struct SelectionBox
{
public:
	SelectionBox(int w, int h, std::vector<T *>& collection);

	void draw(Screen&, int, int, bool) const;
	void move(int x);

	T *getSelected(void)
	{
		return m_Collection[m_Selected];
	}

private:
	int m_Scroll, m_Selected;
	int m_Width, m_Height;
	std::vector<T *>& m_Collection;
};

void recalculate_stats(int *stats, Race *race, Proffesion *proffesion, int level);

Character *create_character(Screen& screen);
