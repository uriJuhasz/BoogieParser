#include "BoogieLexer.h"
#include <cassert>
using namespace std;

namespace BoogieParser {

	BoogieLexer::BoogieLexer(istream& i)
		: ParserBase(i), _curToken(nullptr)
	{
	}

	BoogieLexer::~BoogieLexer() {
		if (_curToken!=nullptr)
			delete _curToken;
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

	bool isSimpleIdentifierStart(BoogieLexer::Char c){
		return
				(c >= 'a' && c < 'z')
				|| (c >= 'A' && c < 'Z')
				|| (c=='_')
				|| (c=='$')
				|| (c=='%')
				|| (c=='.')
				|| (c=='#')
				|| (c=='\'')
				|| (c=='`')
				|| (c=='~')
				|| (c=='^')
				|| (c=='\\')
				|| (c=='?');
	}
	bool isSimpleIdentifierChar(BoogieLexer::Char c){
		return isSimpleIdentifierStart(c);
	}
	bool isQuotedIdentifierStart(BoogieLexer::Char c){
		return c=='|';
	}
	bool isStringLiteralStart(BoogieLexer::Char c){
		return c=='"';
	}
	bool isNumberStart(BoogieLexer::Char c){
		return c>='0' && c<='9';
	}

	void BoogieLexer::getToken(){
		skipWSs();
		ensureNotDone();
		auto c = curChar();
		if (isSimpleIdentifierStart(c))
			getSimpleIdentifier();
		else if (isQuotedIdentifierStart(c))
			getQuotedIdentifier();
		else if (isStringLiteralStart(c))
			getStringLiteral();
		else if (isNumberStart(c))
			getNumber();
		else
			getSymbol();
	}

	void BoogieLexer::startToken(){
		assert(!curTokenStarted);
		curTokenStartPos = curPos();
		curTokenText = "";
		curTokenStarted=true;
	}
	void BoogieLexer::endToken(Token::Kind kind){
		assert(curTokenStarted);
		curTokenStarted=false;
		curToken = new Token(kind,curTokenText,curTokenStartPos,curTokenEndPos);
	}
	void BoogieLexer::getTokenChar(){
		std::assert(curTokenStarted);
		curTokenEndPos = curPos();
		getChar();
	}
/*	const unordered_map<string,Token::Kind> rwMap;
	void BoogieLexer::getSimpleIdentifier(){
		startToken();
		while (isSimpleIdentifierChar(curChar()))
			getTokenChar();

		endToken()
	}*/

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
