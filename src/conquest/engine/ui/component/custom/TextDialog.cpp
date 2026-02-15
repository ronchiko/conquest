#include <ranges>

#include "conquest/engine/ui/Ticker.h"

#include "conquest/engine/ui/component/custom/TextDialog.h"


namespace conquest::window::component {

TextDialogComponent::TextDialogComponent(std::string text)
	: m_Ticker([&] { onTick(); })
	, m_Offset(0)
	, m_Text(std::move(text))
{
	m_Lines.emplace_back();
}

ftxui::Element TextDialogComponent::render()
{
	auto elements = m_Lines
		| std::views::transform([](auto& line) { return ftxui::text(line.str()) | ftxui::center; });

	return ftxui::vbox(ftxui::Elements(elements.begin(), elements.end()));
}

void TextDialogComponent::animate(ftxui::animation::Duration duration)
{
	m_Ticker.tick();
}

bool TextDialogComponent::onEvent(const event::ConquestEvent& event)
{
	return false;
}


void TextDialogComponent::onTick()
{
	if(m_Text.length() <= m_Offset) {
		return;		
	}

	if(const auto character = m_Text[m_Offset++]; '\n' != character) {
		m_Lines.back() << character;
		return;
	}

	m_Lines.emplace_back();
}


ftxui::Component dialog(const std::string& text)
{
	return ftxui::Make<TextDialogComponent>(text);
}

}