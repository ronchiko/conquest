#include <ftxui/component/component.hpp>

#include "conquest/common/Convert.h"
#include "conquest/engine/ui/Manager.h"
#include "conquest/engine/ui/component/Size.h"
#include "conquest/engine/ui/component/Spacer.h"
#include "conquest/engine/ui/component/text/Title.h"
#include "conquest/engine/ui/component/text/Prefix.h"
#include "conquest/engine/ui/component/textbox/Textbox.h"
#include "conquest/engine/ui/component/button/Reflective.h"
#include "conquest/engine/ui/component/custom/StatView.h"
#include "conquest/engine/ui/component/button/Choice.h"
#include "conquest/engine/ui/component/button/Button.h"

#include "conquest/scenes/newGame/CharacterCreator.h"

namespace conquest::scenes::newGame::creator {

namespace {

constexpr auto DESCRIPTION_WIDTH = 60;
constexpr auto DESCRIPTION_HEIGHT = 3;

constexpr auto CHOICE_MENU_WIDTH = 15;

struct CharacterCreationInfo
{
	std::string name;
	ui::Choice<Profession> profession = ui::choice::fromAssetSingleton<Profession>();
	ui::Choice<Race> race = ui::choice::fromAssetSingleton<Race>();
	Stats<int64_t> customStats;

	[[nodiscard]] const Profession& currentProfession() const
	{
		return profession.current();
	}

	[[nodiscard]] const Race& currentRace() const
	{
		return race.current();
	}

	[[nodiscard]] bool canContinue() const
	{
		return !name.empty() && profession.didChoose() && race.didChoose();
	}
};

using Context = window::Context<CharacterCreationInfo>;


ftxui::Component createMainWindow(Context& context)
{
	return ftxui::Container::Vertical({
		ui::title("Create Character") | ftxui::center,
		ui::vspacer(1),
		ftxui::Container::Horizontal({
			ftxui::Container::Vertical({
				ui::title("Basic Information", ftxui::Color::BlueLight),
				ui::vspacer(1),
				ui::prefix("Character Name: ",
						   ui::textBox(context.name,
									   "Enter Name...",
									   ui::Limit{
										   .characters = 24,
										   .multiline = false,
									   })),
				ui::prefix("Profession:    ", reflect(context.profession, context.then_push("profession"))),
				ui::prefix("Race:          ", reflect(context.race, context.then_push("race"))),
			}),
		}),
		ui::vspacer(std::nullopt),
		ftxui::Container::Horizontal({
			ui::hspacer(std::nullopt),
			ui::conditionalButton("Create Character", context.then_push("confirm"), [&] { return context.canContinue(); }),
		}),
	});
}

ftxui::Component createProfessionSelectionMenu(Context& context)
{
	auto menuOptions = ftxui::MenuOption::VerticalAnimated();
	menuOptions.on_enter = [&context] {
		context.pop();
	};

	const auto group = ftxui::Container::Horizontal({
		context.profession.createMenu(menuOptions) | ui::widerThan(CHOICE_MENU_WIDTH),
		ftxui::Renderer([] { return ftxui::separatorDashed(); }),
		ftxui::Container::Vertical({
			ftxui::Renderer([&context] {
				return ftxui::paragraph(context.currentProfession().description()) | ui::width(DESCRIPTION_WIDTH) |
					   ui::height(DESCRIPTION_HEIGHT);
			}),
			ftxui::Renderer([] { return ftxui::separatorDashed(); }),
			ftxui::Container::Horizontal({ ftxui::Renderer([&context] {
											   return statSheet(context.currentProfession().base(),
																ui::StatSheetOptions::highlightValue(0));
										   }) | ftxui::flex,
										   ftxui::Renderer([] { return ftxui::separatorDashed(); }),
										   ftxui::Renderer([&context] {
											   auto weapons = ftxui::Elements{ ftxui::text("Weapons"),
																			   ftxui::separator() };
											   for(const auto& weapon : context.currentProfession().weapons()) {
												   weapons.emplace_back(ftxui::text(weapon.name()) |
																		color(ftxui::Color::Grey53));
											   }

											   return ftxui::vbox(std::move(weapons));
										   }) }),
		}),
	});

	return ftxui::Renderer(group,
						   [=] {
							   return ftxui::vbox({
								   ftxui::text("Select Profession") | ftxui::hcenter,
								   ftxui::separatorDashed(),
								   group->Render(),
							   });
						   }) |
		   ftxui::borderDashed;
}

ftxui::Component createRaceSelectionMenu(Context& context)
{
	auto menuOptions = ftxui::MenuOption::VerticalAnimated();
	menuOptions.on_enter = [&context] {
		context.pop();
	};

	const auto group = ftxui::Container::Horizontal({
		context.race.createMenu(menuOptions) | ui::widerThan(CHOICE_MENU_WIDTH),
		ftxui::Renderer([] { return ftxui::separatorDashed(); }),
		ftxui::Container::Vertical({
			ftxui::Renderer([&context] {
				return ftxui::paragraph(context.currentRace().description())
			        | ui::width(DESCRIPTION_WIDTH)
			        | ui::height(DESCRIPTION_HEIGHT);
			}),
			ftxui::Renderer([] { return ftxui::separatorDashed(); }),
			ftxui::Container::Horizontal({
				ftxui::Renderer([&context] {
					return statSheet(context.race.current().base(), ui::StatSheetOptions::highlightValue(0));
				}) | ftxui::flex,
			}),
		}),
	});

	return ftxui::Renderer(group, [=] {
	   return ftxui::vbox({
		   ftxui::text("Select Race") | ftxui::hcenter,
		   ftxui::separatorDashed(),
		   group->Render(),
	   });
    }) | ftxui::borderDashed;
}

ftxui::Component createConfirmPopup(Context& context)
{
	return ftxui::Container::Vertical({
		ui::title("Confirm Character Traits", ftxui::Color::Yellow) | ftxui::hcenter,
		ftxui::Renderer([] { return ftxui::separator();}),
		ui::text("Are you sure you want to continue?") | ftxui::hcenter,
		ui::text("You will not be able to change these attributes later") | ftxui::hcenter,
		ui::vspacer(1),
		ftxui::Container::Horizontal({
			ui::button("Go Back", [&] { context.pop();}),
			ui::button("Confirm", [&] { context.pop(true);}),
		}) | ftxui::hcenter,
    }) | ftxui::border;

}

}

Character createCharacter()
{
	auto window = window::manager<CharacterCreationInfo>(createMainWindow)
        .add("profession", createProfessionSelectionMenu)
		.add("race", createRaceSelectionMenu)
		.add("confirm", createConfirmPopup);

	const auto creationInfo = static_cast<CharacterCreationInfo>(window.start());
	return Character{
		CharacterTraits{ creationInfo.currentRace(), creationInfo.currentProfession() },
		CharacterAttributes{ creationInfo.name, creationInfo.customStats },
		CharacterExperience{ CharacterExperienceRate::Normal, 1 },
	};
}

}
