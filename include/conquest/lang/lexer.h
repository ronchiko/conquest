#pragma once

#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "conquest/types.h"
#include "conquest/message.h"
#include "conquest/traits/variant.h"

namespace conquest::lang {
	struct Token
	{
		enum class Type
		{
			Identifier,
			Keyword,

			Newline,
			Integer,
			String,
			Decimal,

			Colon,
			Comma,
			Precentage,
			Plus,
			Minus
		};

		Token(Type type)
			: type(type)
			, value(std::monostate{})
		{}

		explicit Token(Type type, std::string str)
			: type(type)
			, value(std::move(str))
		{}

		explicit Token(int32 number)
			: type(Type::Integer)
			, value(number)
		{}

		explicit Token(float number)
			: type(Type::Decimal)
			, value(number)
		{}

		template<typename...Types>
		constexpr bool is(Types... types) const
		{
			static_assert((std::is_same_v<Types, Type> && ...), "Only accepts Token::Type");

			return ((type == types) || ...);
		}

		
		template<typename Type>
		constexpr Type& as()
		{
			static_assert(is_variant_alternate_v<Type, decltype(value)>, "Invalid Type");

			return std::get<Type>(value);
		}

		template<typename Type>
		constexpr const Type& as() const
		{
			static_assert(is_variant_alternate_v<Type, decltype(value)>, "Invalid Type");

			return std::get<Type>(value);
		}

		Type type;
		std::variant<int32, float, std::string, std::monostate> value;
	};

	struct LexingResult
	{
		bool isOk = true;
		MessageQueue messages;
		std::vector<Token> tokens;

		/**
		 * Fails this result with a message.
		 *
		 * @param message - The failure message.
		 */
		template<typename... Args>
		LexingResult& fail(Args&&... args)
		{
			messages.add("ERROR: ", args...);
			isOk = false;
			return *this;
		}
	};

	class Lexer
	{
	public:
		explicit Lexer(const std::string& text);

		LexingResult parseTokens();

	private:
		void commitToken(Token token);
		void pushMemory();

		LexingResult mResult;
		std::stringstream mStream;
		std::stringstream mMemory;
	};
}