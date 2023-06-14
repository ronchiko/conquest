#include "conquest/lang/parser.h"

#include <unordered_map>

namespace conquest::lang {

	static const std::unordered_map<std::string, Object::Type> OBJECT_TYPES{
		{ "element", Object::Type::Element },
		{ "weapon_type", Object::Type::WeaponType },
		{ "race", Object::Type::Race },
		{ "profession", Object::Type::Profession },
		{ "move", Object::Type::Move },
	};

	Parser::Parser(std::vector<Token> tokens)
		: mResult()
		, mTokens(std::move(tokens))
	{}

	ParseResult Parser::parse()
	{
		while(!mTokens.eof()) {
			if (ParseResult::Status::Ok != parseObject()) {
				if(!mTokens.eof()) {
					mTokens.advance();
				}

				mResult.isOk = false;
			}
		}

		return mResult;
	}

	ParseResult::Status Parser::parseObject()
	{
		if(!skip(Token::Type::Newline)) {
			return ParseResult::Status::Ok;
		}

		auto objectTypeToken = mTokens.advance();
		if(!objectTypeToken.is(Token::Type::Keyword)) {
			return mResult.fail("Expected keyword!");
		}

		const auto type = OBJECT_TYPES.find(objectTypeToken.as<std::string>());
		if(OBJECT_TYPES.end() == type) {
			return mResult.fail(objectTypeToken.as<std::string>(), " is not a valid object type");
		}

		auto objectName = mTokens.advance();
		if(!objectName.is(Token::Type::Identifier, Token::Type::String)) {
			return mResult.fail("Expected identifier or String");
		}

		if(!mTokens.advance().is(Token::Type::Colon)) {
			return mResult.fail("Expeted a colon");
		}

		Object object;
		object.type = type->second;
		object.name = objectName.as<std::string>();

		// TODO: Parse modifiers and inheritence.

		if(!mTokens.advance().is(Token::Type::Newline)) {
			return mResult.fail("Expected newline after object header");
		}

		while (!mTokens.eof()) {
			auto status = parseAttribute();
			if(status.isError()) {
				if(ParseResult::Status::Break == status.error()) {
					break;
				}

				return ParseResult::Status::Failure;
			}

			object.attributes.insert(std::move(status.value()));
		}

		mResult.objects.emplace(object.name, std::move(object));
		return ParseResult::Status::Ok;
	}

	Result<std::pair<std::string, Attribute>, ParseResult::Status> Parser::parseAttribute()
	{
		if (!skip(Token::Type::Newline)) {
			return ParseResult::Status::Break;
		}

		if(mTokens.eof() || !mTokens.peek().is(Token::Type::Identifier)) {
			return ParseResult::Status::Break; // Finished the object.
		}

		const auto& attributeToken = mTokens.advance();
		
		auto expression = parseAttributeList();
		if(expression.isError()) {
			return expression.error();
		}

		if(!mTokens.eof() && !mTokens.advance().is(Token::Type::Newline)) {
			return mResult.fail("Expected newline after attribute.");
		}

		return std::make_pair(attributeToken.as<std::string>(), std::move(expression.value()));
	}

	Result<Attribute, ParseResult::Status> Parser::parseAttributeList() {
		std::vector<Attribute::BaseAttribute> list;

		do {
			auto literal = parseLiteral();
			if (literal.isError()) {
				return literal.error();
			}

			list.emplace_back(std::move(literal.value()));
		} while(advanceIf(Token::Type::Comma));

		return Attribute(std::move(list));
	}

	Result<Attribute::BaseAttribute, ParseResult::Status> Parser::parseLiteral()
	{
		auto& token = mTokens.advance();
		switch (token.type) {
		case Token::Type::Plus:
		{
			auto& value = mTokens.advance();
			return Attribute::BaseAttribute(value.as<int32>());
		} break;
		case Token::Type::Minus:
		{
			auto& value = mTokens.advance();
			return Attribute::BaseAttribute(-value.as<int32>());
		} break;

		case Token::Type::Integer:
			return Attribute::BaseAttribute(token.as<int32>());
		case Token::Type::Decimal:
			return Attribute::BaseAttribute(token.as<float>());
		case Token::Type::Identifier:
		case Token::Type::String:
			return Attribute::BaseAttribute(std::move(token.as<std::string>()));
		}

		return mResult.fail("Expected identifier, string, decimal or interger");
	}
}
