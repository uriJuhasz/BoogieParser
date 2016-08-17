#include "BoogieLexer.h"
using namespace std;

namespace BoogieParser {

	BoogieLexer::BoogieLexer(istream& i)
		: ParserBase(i), _curToken(nullptr)
	{
	}

	BoogieLexer::~BoogieLexer() {
		if (_curToken!=nullptr)
			delete curToken();
	}

	void BoogieLexer::ensureNotDone()
	{
		if (done())
			throw new ReadPastEOFException(curPos());
	}
	Token BoogieLexer::curToken(){
		ensureNotDone();
		if (!_curToken)
			getToken();
		return *_curToken;
	}

	void BoogieLexer::nextToken()
	{
		getToken();
	}

	void BoogieLexer::getToken(){
		ensureNotDone();
		auto startPos = curPos();
		auto c = curChar();
		switch (c){
			case '(' : getChar(); ,setToken(new Token(leftParens,startPos)))
		}
	}

} /* namespace BoogieParser */


/*
const std::locale loc;
bool isEOL(Char c){
	return c=='\n';
}
bool isWhiteSpace(Char c){
	return isspace(c,loc);
}
typedef tuple<Size,Position> CPos;
{
	posStack.push(tuple<Size,Position>(bufPos,curPos));
}

void skipWSs()
{
	while (!done() && isWhiteSpace(peekChar()))
		getChar();
}
*/
