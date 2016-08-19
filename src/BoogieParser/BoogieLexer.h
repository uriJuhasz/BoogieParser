/*
 * BoogieLexer.h
 *
 *  Created on: Aug 17, 2016
 *      Author: uri
 */

#ifndef INCLUDE_BOOGIELEXER_H_
#define INCLUDE_BOOGIELEXER_H_

#include "ParserBase.h"
#include "Position.h"

namespace BoogieParser {

	class Token{
	public:
		enum class Kind{
			typeRW,
			varRW,
			constRW,
			procedureRW,
			implementationRW,
			axiomRW,
			functionRW,

			returnsRW,

			assertRW,
			assumeRW,
			breakRW,
			callRW,
			ifRW,
			thenRW,
			elseRW,
			whileRW,
			returnRW,
			havocRW,

			ensuresRW,
			requiresRW,
			invariantRW,
			modifiesRW,

			whereRW,

			forallRW,
			existsRW,

			finiteRW,
			freeRW,


			boolRW,
			intRW,

			identifier,
			bvx,

			integer,


			leftCurly,
			rightCurly,
			leftSquare,
			rightSquare,
			leftParen,
			rightParen,

			dot,
			comma,
			colon,
			semicolon,
			assign
		};

		Token(Kind _kind, const std::string& _text,const Position& _pos,const Position& _endPos)
			: kind(_kind), text(_text), pos(_pos),endPos(_endPos)
			  {}
		const Kind kind;
		const std::string text;
		const Position pos,endPos;
	};

	class BoogieLexer: private ParserBase {
	public:
		typedef ParserBase::Char Char;
		BoogieLexer(std::istream& i);
		virtual ~BoogieLexer();

		bool eof();
		Position curPos();

		Token curToken();
		void nextToken();
	private:
		void getToken();
		void getSimpleIdentifier();
		void getQuotedIdentifier();
		void getNumber();
		void getStringLiteral();
		void getSymbol();

		void ensureNotDone();

		void startToken();
		Char getTokenChar();
		void endToken(Token::Kind kind);

		void skipWSs();

		Token* _curToken = nullptr;
		std::string curTokenText;
		bool curTokenStarted = false;
		Position curTokenStartPos;
		Position curTokenEndPos;
	};

} /* namespace BoogieParser */

#endif /* INCLUDE_BOOGIELEXER_H_ */
