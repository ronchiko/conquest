#pragma once

#include <variant>

namespace conquest::scenes::title {

enum class ResultType
{
	NewGame,
	ContinueGame,

	Exit,
};

struct Result
{
	struct New {};
	struct Exit {};

	struct Load
	{
		std::string saveName;
	};

	ResultType type;
	std::variant<New, Load, Exit> content;

	constexpr void newGame()
	{
		type = ResultType::NewGame;
		content = New{};
	}

	constexpr void loadGame(std::string saveName)
	{
		type = ResultType::ContinueGame;
		content = Load{ std::move(saveName) };
	}

	constexpr void exit()
	{
		type = ResultType::Exit;
		content = Exit{};
	}
};

/**
	Shows the current title screen.
 */
Result show();

}
