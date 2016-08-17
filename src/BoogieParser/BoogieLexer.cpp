#include "BoogieLexer.h"

namespace BoogieParser {

BoogieLexer::BoogieLexer() {
	// TODO Auto-generated constructor stub

}

BoogieLexer::~BoogieLexer() {
	// TODO Auto-generated destructor stub
}

} /* namespace BoogieParser */



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

