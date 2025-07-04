#pragma once

#include <stack>

#include <ftxui/component/screen_interactive.hpp>

#include "conquest/engine/window/internal/ModalStorage.h"


namespace conquest::window {


template<typename T>
class Context : public T
{
public:
	explicit Context(ftxui::ScreenInteractive& screen, internal::ModalStorage& storage)
		: m_Screen(&screen)
		, m_Storage(&storage)
	{}

	void push(const std::string& name)
	{
		const auto activator = m_Storage->activatorOf(name);
		*activator = true;
		m_Stack.push(activator);
	}

	void pop(bool full = false)
	{
		if(full || m_Stack.empty()) {
			m_Screen->Exit();
			return;
		}

		*m_Stack.top() = false;
		m_Stack.pop();
	}

private:
	ftxui::ScreenInteractive* m_Screen;
	internal::ModalStorage *m_Storage;

	std::stack<bool *> m_Stack;
};

}