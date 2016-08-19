/*
 * ParserBase.h
 *
 *  Created on: Aug 17, 2016
 *      Author: uri
 */

#ifndef INCLUDE_PARSERBASE_H_
#define INCLUDE_PARSERBASE_H_

#include <exception>
#include <istream>
#include <string>
#include <ios>
#include <vector>
#include <locale>
#include <stack>
#include "Position.h"

namespace BoogieParser {

	class ParserBase {
		public:
			class ParserException : public std::exception{
				public:
					ParserException(const Position _pos);
					const Position pos;
					virtual std::string message();

			};
			class ReadPastEOFException : public ParserException{
				public:
					ReadPastEOFException(const Position _pos);
					virtual std::string message();
			};
			class StreamReadException : public ParserException{
				public:StreamReadException(const Position _pos);
			};
		protected:

			typedef char Char;
			typedef std::streamsize Size;

			ParserBase(std::istream& _f);
			virtual ~ParserBase();

			bool done() const;

			Char getChar();
			Char curChar();

			Position curPos() const;
			void pushPos();
			void popPos();
			void discardPopPos();

        static bool isEOL(Char c);
		private:
			class CPos;
			bool bufDone() const;
			void readToBuf();



			std::istream& f;
			std::vector<Char> buf;
			Size bufPos = 0;

			std::stack<CPos> posStack;

			Position _curPos = Position(1,1);
		};

} /* namespace BoogieParser */

#endif /* INCLUDE_PARSERBASE_H_ */
