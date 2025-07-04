#include "conquest/scenes/title/TitleScreen.h"
int main(void)
{
	using namespace conquest;

	const auto titleScreenChoice = scenes::title::show();
	if(scenes::title::ResultType::Exit == titleScreenChoice.type) {
		return -1;
	}

	return 0;
}
