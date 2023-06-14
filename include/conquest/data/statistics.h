#pragma once

#include <numeric>
#include <string>

#include "conquest/types.h"

namespace conquest {
	using Stat = uint16;

	enum class StatType
	{
		Hp = 0, // [HP] The hit points of the character (Reduced by taking damage)
		Energy, // [ENRG] The energy of the character (Reduced by using moves)

		// Overworld stats - Effects overload events
		Fame,	  // [FAME] The fame of the character (Effect random events + prices)
		Charisma, // [CHRS] Increases chance to convince characters

		// Primary battle stats - Effects damage given and taken in battle
		Strength,	// [STRG] The strength of the character (Increases incoming physical damage)
		Defence,	// [DEFC] The defence of the character (Reduces recieved physical resistance)
		Intellect,	// [INT] Same as strength but for magical attacks
		Resistance, // [RES] Same as defence but for magical attacks

		// Secondary battle stats - Other battle stats
		Speed,	 // [SPD] Effects the how often the character attacks and the chance to evade attacks
		Stealth, // [STLH] Increases chance for critical hits and reduces miss chance.

		// Generic stats - Effects both battle and overworld stuff
		Luck, // [LUCK] Slightly increases evasion and crit chance and reduces store prices,
			  // also increases chance to get loot and shop items.

		// -- END --
		_Final, // Marks the end of the stats lis
	};

	/**
	   Gets a stat by its short name.

	   @param name - The name of the stat.
	 */
	StatType getStatFromName(const std::string& name);

	template<typename ChartT = Stat>
	class StatChart
	{
	public:
		union
		{
			struct
			{
				ChartT hp;
				ChartT energy;

				ChartT fame;
				ChartT charisma;

				ChartT strength;
				ChartT defence;
				ChartT intellect;
				ChartT resistance;

				ChartT speed;
				ChartT stealth;

				ChartT luck;
			};

			ChartT all[static_cast<uint32>(StatType::_Final)] = { 0 };
		};

		/**
		   The total value stats of the stats in this chart.
		 */
		inline uint32 total() const
		{
			return std::accumulate(std::begin(all), std::end(all), 0);
		}

		constexpr ChartT& operator[](const StatType stat)
		{
			return all[static_cast<uint32>(stat)];
		}

		constexpr const ChartT& operator[](const StatType stat) const
		{
			return all[static_cast<uint32>(stat)];
		}
	};
}
