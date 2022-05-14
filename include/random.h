#pragma once

#include <stdlib.h>
#include <time.h>

#define FLT_ACCURACY 10000

class Random {
public:
	class _rand_instance {
		_rand_instance() {
			srand(time(NULL));
		}
	};

	static float value(void) {
		return static_cast<float>(rand() % FLT_ACCURACY) / FLT_ACCURACY;
	}
	static float float32(float min, float max) {
		return (max - min) * random_value() + min;
	}
	static int int32(int min, int max) {
		return (rand() % (max - min)) + min;
	}

	static _rand_instance _instance;
};
