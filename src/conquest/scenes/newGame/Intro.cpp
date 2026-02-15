#include <sstream>

#include <ftxui/component/animation.hpp>

#include "conquest/engine/ui/Manager.h"
#include "conquest/engine/ui/Ticker.h"

#include "conquest/scenes/newGame/Intro.h"


namespace conquest::scenes::newGame::intro {

	class Typing : public ftxui::ComponentBase
	{
	public:
		explicit Typing(std::string text)
			: m_Ticker([&] { onTick(); })
			, m_Offset(0)
			, m_Text(std::move(text))
		{
			m_Lines.emplace_back();
		}

		ftxui::Element OnRender() override
		{
			ftxui::Elements lines;
			lines.reserve(m_Lines.size());
			for(const auto& line : m_Lines) {
				lines.push_back(
					ftxui::text(line.str()) | ftxui::hcenter
				);
			}

			return ftxui::vbox(lines);
		}

		void OnAnimation(ftxui::animation::Params& params) override
		{
			m_Ticker.tick();
		}

		bool OnEvent(ftxui::Event event) override
		{
			if(event != ftxui::Event::Custom) {
				return false;
			}

			m_Ticker.restart(m_Text.length());
			return false;
		}

	private:
		void onTick() {
			if(m_Text.length() <= m_Offset) {
				return;
			}

			const auto character = m_Text[m_Offset++];
			if(character == '\n') {
				m_Lines.emplace_back();
				return;
			}

			m_Lines.back() << character;
		}

		window::Ticker m_Ticker;
		std::vector<std::stringstream> m_Lines;

		size_t m_Offset;
		std::string m_Text;
	};

ftxui::Component createMainComponent(window::Context<Response>&)
{
	return ftxui::Make<Typing>("WTF this is a very long a combersome piece of text\n"
		"If you wish to stop reading this bitch ass piece of text than you can press\n"
		"any key on you keyboard dude.\n\nsybau");
}

Response show()
{
	auto window = window::manager<Response>(createMainComponent);

	return window.start();
}

}
