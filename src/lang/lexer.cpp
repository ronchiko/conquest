#include "conquest/lang/lexer.h"

#include <cctype>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace conquest::lang {

	static const std::unordered_set<std::string> KEYWORDS{
		"element",
		"weapon_type",
		"race",
		"profession",
	};

	static const std::unordered_map<char, Token::Type> SIMPLE_TOKENS{
		{ ':', Token::Type::Colon }, { ',', Token::Type::Comma }, { '%', Token::Type::Precentage },
		{ '+', Token::Type::Plus },	 { '-', Token::Type::Minus },
	};

	static bool isAlphanumeric(const char value)
	{
		return std::isalnum(value) || value == '_';
	}

	static void consumeLine(std::stringstream& stream)
	{
		std::string message;
		std::getline(stream, message);
	}

	static std::variant<int32, float, std::string> inferNumberType(const std::string& str)
	{
		float decimalResult = 0.f;

		// This works by letter STL parse the number as a floating point and then if
		// the checking if its an actually an integer.
		uint32 factor = 0;
		uint32 numeric = 0;
		for(const auto& p : str) {
			if(std::isdigit(p)) {
				factor *= 10;
				numeric = numeric * 10 + (p - '0');
			} else if('.' == p) {
				if(0 != factor) {
					return str;
				}
				factor = 1;
			} else {
				return str;
			}
		}

		if(0 != factor) {
			return static_cast<float>(numeric) / factor;
		}

		return static_cast<int32>(numeric);
	}

	Lexer::Lexer(const std::string& text)
		: mResult()
		, mStream(text)
		, mMemory()
	{}

	LexingResult Lexer::parseTokens()
	{
		while(!mStream.eof()) {
			char current = 0;
			mStream.read(&current, std::streamsize(1));

			if(0 == current) {
				break;
			}

			// Our character is the blank character. keep going.
			if(isspace(current)) {
				if('\n' == current) { // We need to remember newlines
					commitToken(Token::Type::Newline);
				}
				pushMemory();
				continue;
			}

			switch(current) {
			case '#': // # Marks the start of a comment, so we can just ignore everything after it.
				consumeLine(mStream);
				commitToken(Token::Type::Newline);
				break;
			case '"':
			{
				std::stringstream string;
				do {
					mStream.read(&current, 1);
					if('"' == current) {
						break;
					}

					if(mStream.eof() || '\n' == current) {
						return mResult.fail("Unterminted string");
					}

					string << current;
				} while(true);

				commitToken(Token(Token::Type::String, string.str()));
				break;
			}
			default:
			{
				const auto iterator = SIMPLE_TOKENS.find(current);
				if(SIMPLE_TOKENS.end() != iterator) {
					commitToken(iterator->second);
					break;
				}

				// Only . and alpha numeric character can be stored in memory.
				if(!isAlphanumeric(current) && '.' != current) {
					pushMemory();
					return mResult.fail("Illegal token: '", current, "' (0x", std::hex, static_cast<uint32>(current), ")");
				}

				mMemory << current;
				break;
			}
			}
		}

		return mResult;
	}

	void Lexer::commitToken(Token token)
	{
		pushMemory();
		mResult.tokens.push_back(std::move(token));
	}

	void Lexer::pushMemory()
	{
		auto identifier = mMemory.str();
		mMemory.str("");

		if(identifier.empty()) {
			return;
		}

		const bool isDot = '.' == identifier.at(0);
		const bool isDigit = std::isdigit(identifier.at(0));
		if(!isDigit && !isDot) { // If its not a digit or a dot, it must be an identifier
			const auto type = KEYWORDS.end() != KEYWORDS.find(identifier) ? Token::Type::Keyword
																		  : Token::Type::Identifier;
			mResult.tokens.emplace_back(type, std::move(identifier));
			return;
		}

		const auto convertedValue = inferNumberType(identifier);
		auto token = std::visit(Overload{
									[](float value) -> std::optional<Token> { return Token(value); },
									[](int32 value) -> std::optional<Token> { return Token(value); },
									[&](const std::string& value) -> std::optional<Token> {
										mResult.fail("Number didn't match any patterns: ", value);
										return std::nullopt;
									},
								},
								convertedValue);

		if(!token.has_value()) {
			return;
		}

		mResult.tokens.push_back(std::move(token.value()));
	}
}