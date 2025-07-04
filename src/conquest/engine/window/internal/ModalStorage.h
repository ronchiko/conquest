#pragma once

#include <unordered_map>

#include <ftxui/component/component.hpp>


namespace conquest::window::internal {

class ModalStorage
{
public:
	/**
		Adds a new modal to storage.

		@param name - The name of the modal.
		@param component - The modal component to add.
		@param parentComponent - The main component to add the modal too.
	 */
	void add(const std::string& name, ftxui::Component component, ftxui::Component& parentComponent);

	/**
		Returns the address to activator boolean of a modal.

		@param name - The name of the modal to get the activator of.
	 */
	bool *activatorOf(const std::string& name);

private:
	struct Modal
	{
		bool activator;
		ftxui::Component component;
	};

	std::unordered_map<std::string, Modal> m_Modals;
};

}
