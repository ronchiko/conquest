#include "conquest/engine/ui/internal/ModalStorage.h"


namespace conquest::window::internal {

void ModalStorage::add(const std::string& name, ftxui::Component component, ftxui::Component& parentComponent)
{
	auto& [activator, modalComponent] = (m_Modals[name] = Modal{ false, std::move(component) });
	parentComponent |= ftxui::Modal(modalComponent, &activator);
}

bool* ModalStorage::activatorOf(const std::string& name)
{
	return &m_Modals[name].activator;
}

}