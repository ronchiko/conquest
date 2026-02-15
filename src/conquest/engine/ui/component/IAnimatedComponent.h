#pragma once

#include "conquest/engine/ui/component/IComponent.h"


namespace conquest::window::component {

class IAnimatedComponent : virtual public IComponent
{
public:
	virtual void animate(ftxui::animation::Duration duration) = 0;

private:
	void OnAnimation(ftxui::animation::Params& params) final {
		animate(params.duration());
	}
};

}
