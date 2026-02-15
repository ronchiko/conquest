#pragma once

#include <ftxui/component/component.hpp>


namespace conquest::window::component {

class IComponent : virtual public ftxui::ComponentBase
{
public:
	virtual ftxui::Element render() = 0;
	
private:
	ftxui::Element OnRender() final { return render(); }
};

}
