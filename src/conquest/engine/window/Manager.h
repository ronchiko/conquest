#pragma once

#include <ftxui/component/component.hpp>

#include "conquest/engine/window/Context.h"
#include "conquest/engine/window/internal/ModalStorage.h"

namespace conquest::window {

namespace internal
{

struct Screen
{
	ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
};

using ScreenPtr_t = std::unique_ptr<Screen>;
using Storage_t = std::unique_ptr<internal::ModalStorage>;

}

template<typename ResponseT>
class Manager
{
public:
	using Creator_t = std::function<ftxui::Component(Context<ResponseT>&)>;

	static Manager create(Creator_t mainComponentCreator);

	Manager& add(std::string name, Creator_t modalCreator) &;
	Manager&& add(const std::string& name, Creator_t modalCreator) &&;

	ResponseT& start();
	
private:
	using ContextPtr_t = std::unique_ptr<Context<ResponseT>>;

	explicit Manager(ftxui::Component root, internal::Storage_t storage, internal::ScreenPtr_t screen, ContextPtr_t context)
		: m_MainComponent(std::move(root))
		, m_ModalStorage(std::move(storage))
		, m_Screen(std::move(screen))
		, m_Context(std::move(context))
	{}

	ftxui::Component m_MainComponent;
	internal::Storage_t m_ModalStorage;
	internal::ScreenPtr_t m_Screen;
	ContextPtr_t m_Context;
};

template<typename ResponseT>
Manager<ResponseT> manager(typename Manager<ResponseT>::Creator_t creator)
{
	return Manager<ResponseT>::create(creator);
}

template <typename ResponseT>
Manager<ResponseT> Manager<ResponseT>::create(Creator_t mainComponentCreator)
{
	auto screen = std::make_unique<internal::Screen>();
	auto storage = std::make_unique<internal::ModalStorage>();
	auto context = std::make_unique<Context<ResponseT>>(screen->screen, *storage);

	auto root = mainComponentCreator(*context);

	return Manager{
		std::move(root),
		std::move(storage),
		std::move(screen),
		std::move(context),
	};
}

template <typename ResponseT>
Manager<ResponseT>& Manager<ResponseT>::add(std::string name, Creator_t modalCreator) &
{
	m_ModalStorage->add(std::move(name), modalCreator(*m_Context), m_MainComponent);
	return *this;
}

template <typename ResponseT>
Manager<ResponseT>&& Manager<ResponseT>::add(const std::string& name, Creator_t modalCreator) &&
{
	m_ModalStorage->add(name, modalCreator(*m_Context), m_MainComponent);
	return std::move(*this);
}

template <typename ResponseT>
ResponseT& Manager<ResponseT>::start()
{
	m_Screen->screen.Loop(m_MainComponent);
	return *m_Context;
}

}
