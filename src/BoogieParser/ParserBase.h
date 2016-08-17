/*
 * ParserBase.h
 *
 *  Created on: Aug 17, 2016
 *      Author: uri
 */

#ifndef INCLUDE_PARSERBASE_H_
#define INCLUDE_PARSERBASE_H_

#include <exception>
#include "Position.h"

namespace BoogieParser {

	class ParserBase {
		public:
			class ParserException : public std::exception{
				public:
					ParserException(const Position _pos) : pos(_pos){};
					const Position pos;
					virtual std::string message();

			};
			class ReadPastEOFException : public ParserException{
				public:
					ReadPastEOFException(const Position _pos) : ParserException(_pos){}
					virtual std::string message(){return "Read past EOF at " + pos.toString();}
			};
			class StreamReadException : public ParserException{public:StreamReadException(const Position _pos) : ParserException(_pos){}};
			class ParserMismatchException : public ParserException{
				public:
					ParserMismatchException(const Position _pos,const std::string& _expected) : ParserException(_pos), expected(_expected){}
					const std::string expected;
					virtual std::string message(){return "Syntax error at " + pos.toString() + " expected \"" + expected + "\"";}
			};
		protected:
			typedef char Char;
			typedef streamsize Size;
			ParserBase(istream& _f) :f(_f){
				bufPos = 0;
			}
			virtual ~ParserBase(){}

			bool done() const{
				return bufDone() && f.eof();
			}

			Char getChar(){
				Char c = peekChar();
				bufPos++;
				if (isEOL(c))
					curPos = Position(curPos.line+1,0);
				else
					curPos = Position(curPos.line,curPos.col+1);
				return c;
			}
			Char peekChar(){
				if (bufDone())
					readToBuf();
				if (done())
					throw new ReadPastEOFException(curPos);
				Char c = buf[bufPos];
				return c;
			}

			void parseString(const string& s)
			{
				skipWSs();
				auto startPos = curPos;
				unsigned int i = 0;
				while (!done() && i<s.length())
				{
					Char c = getChar();
					if (c!=s[i])
						throw new ParserMismatchException(startPos,s);

				}
			}

			bool tryParseString(const string &s)
			{
				pushPos();
				bool r = true;
				try {
					parseString(s);
				} catch (ParserMismatchException &) {
					r = false;
				}
				if (r)
					discardPopPos();
				else
					popPos();
				return r;
			}
			Position& curPosition() {return curPos;}

		//    class
		private:
			const locale loc;
			bool isEOL(Char c){
				return c=='\n';
			}
			bool isWhiteSpace(Char c){
				return isspace(c,loc);
			}
			typedef tuple<Size,Position> CPos;
			void pushPos()
			{
				posStack.push(tuple<Size,Position>(bufPos,curPos));
			}
			void popPos()
			{
				tuple<Size,Position> t = posStack.top();
				posStack.pop();
				bufPos = get<0>(t);
				curPos = get<1>(t);
			}
			void discardPopPos(){
				posStack.pop();
			}

			Position curPos = Position(0,0);

			void skipWSs()
			{
				while (!done() && isWhiteSpace(peekChar()))
					getChar();
			}

			bool bufDone() const{
				return bufPos==static_cast<Size>(buf.size());
			}
			void readToBuf(){
				while (!f.eof() && !f.good()){
					Position pos = curPos;
					Char c = static_cast<Char>(f.get());
					if (!f.good())
						throw new StreamReadException(pos);
					buf.push_back(c);
				}
			}


			istream& f;
			vector<Char> buf;
			Size bufPos = 0;

			stack<CPos> posStack;
		};

} /* namespace BoogieParser */

#endif /* INCLUDE_PARSERBASE_H_ */
