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

	class BoogieToken{
	public:
		typedef enum class Kind{
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

		BoogieToken(Kind _kind, const std::string& _text,const Position& _pos)
			: kind(_kind), text(_text), pos(_pos)
			  {}
		const Kind kind;
		const std::string text;
		const Position pos;
	};

	class BoogieLexer: public ParserBase {
	protected:
		BoogieLexer();
		virtual ~BoogieLexer();
	};

} /* namespace BoogieParser */

#endif /* INCLUDE_BOOGIELEXER_H_ */
