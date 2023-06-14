#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "conquest/lang/lexer.h"
#include "conquest/lang/object.h"
#include "conquest/message.h"
#include "conquest/result.h"
#include "conquest/stream.h"

namespace conquest::lang {

	struct ParseResult
	{
		enum class Status
		{
			Ok,
			Failure,
			Break
		};

		bool isOk = true;
		MessageQueue messages = {};
		std::unordered_map<std::string, Object> objects = {};

		template<typename... Args>
		Status fail(Args&&...args)
		{
			isOk = false;
			messages.add("ERROR: ", args...);
			return Status::Failure;
		}
	};

	class Parser
	{
	public:
		explicit Parser(std::vector<Token> tokens);

		ParseResult parse();

	private:
		template<typename... Types>
		std::optional<Token> skip(Types... types)
		{
			if (mTokens.eof()) {
				return {};
			}

			Token token = mTokens.peek();
			while(token.is(types...)) {
				mTokens.advance();

				if (mTokens.eof()) {
					return {};
				}

				token = mTokens.peek();
			}

			return token;
		}

		template<typename... Types>
		bool advanceIf(Types... types)
		{
			if(!mTokens.eof() && mTokens.peek().is(types...)) {
				mTokens.advance();
				return true;
			}

			return false;
		}

		ParseResult::Status parseObject();

		Result<std::pair<std::string, Attribute>, ParseResult::Status> parseAttribute();

		Result<Attribute, ParseResult::Status> parseAttributeList();

		Result<Attribute::BaseAttribute, ParseResult::Status> parseLiteral();

		ParseResult mResult;
		Stream<Token> mTokens;
	};
}