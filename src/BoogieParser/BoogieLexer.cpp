#include "BoogieLexer.h"
#include "../util/Trie.h"
#include <cassert>

using namespace std;

namespace BoogieParser {

    BoogieLexer::BoogieLexer(istream &i)
            : ParserBase(i), _curToken(nullptr) {
    }

    BoogieLexer::~BoogieLexer() {
        if (_curToken != nullptr)
            delete _curToken;
    }

    void BoogieLexer::ensureNotDone() {
        if (done())
            throw new ReadPastEOFException(curPos());
    }

    Token BoogieLexer::curToken() {
        ensureNotDone();
        if (!_curToken)
            getToken();
        return *_curToken;
    }

    void BoogieLexer::nextToken() {
        getToken();
    }

    bool isSimpleIdentifierStart(BoogieLexer::Char c) {
        return
                (c >= 'a' && c < 'z')
                || (c >= 'A' && c < 'Z')
                || (c == '_')
                || (c == '$')
                || (c == '%')
                || (c == '.')
                || (c == '#')
                || (c == '\'')
                || (c == '`')
                || (c == '~')
                || (c == '^')
                || (c == '\\')
                || (c == '?');
    }

    bool isSimpleIdentifierChar(BoogieLexer::Char c) {
        return isSimpleIdentifierStart(c);
    }

    bool isQuotedIdentifierStart(BoogieLexer::Char c) {
        return c == '|';
    }

    bool isStringLiteralStart(BoogieLexer::Char c) {
        return c == '"';
    }

    bool isNumberStart(BoogieLexer::Char c) {
        return c >= '0' && c <= '9';
    }

    void BoogieLexer::getToken() {
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

    void BoogieLexer::startToken() {
        assert(!curTokenStarted);
        curTokenStartPos = curPos();
        curTokenText = "";
        curTokenStarted = true;
    }

    void BoogieLexer::endToken(Token::Kind kind) {
        assert(curTokenStarted);
        curTokenStarted = false;
        _curToken = new Token(kind, curTokenText, curTokenStartPos, curTokenEndPos);
    }

    BoogieLexer::Char BoogieLexer::getTokenChar() {
        assert(curTokenStarted);
        curTokenEndPos = curPos();
        return getChar();
    }

    const Trie<BoogieLexer::Char, Token::Kind> rwMap{
            {"type",           Token::Kind::typeRW},
            {"var",            Token::Kind::varRW},
            {"const",          Token::Kind::constRW},
            {"procedure",      Token::Kind::procedureRW},
            {"implementation", Token::Kind::implementationRW},
            {"axiom",          Token::Kind::axiomRW},
            {"function",       Token::Kind::functionRW},

            {"returns",        Token::Kind::returnsRW},

            {"assert",         Token::Kind::assertRW},
            {"assume",         Token::Kind::assumeRW},
            {"break",          Token::Kind::breakRW},
            {"call",           Token::Kind::callRW},
            {"if",             Token::Kind::ifRW},
            {"then",           Token::Kind::thenRW},
            {"else",           Token::Kind::elseRW},
            {"while",          Token::Kind::whileRW},
            {"return",         Token::Kind::returnRW},
            {"havoc",          Token::Kind::havocRW},
            {"goto",           Token::Kind::gotoRW},

            {"ensures",        Token::Kind::ensuresRW},
            {"requires",       Token::Kind::requiresRW},
            {"invariant",      Token::Kind::invariantRW},
            {"modifies",       Token::Kind::modifiesRW},

            {"where",          Token::Kind::whereRW},

            {"forall",         Token::Kind::forallRW},
            {"exists",         Token::Kind::existsRW},

            {"finite",         Token::Kind::finiteRW},
            {"free",           Token::Kind::freeRW},


            {"bool",           Token::Kind::boolRW},
            {"int",            Token::Kind::intRW}
    };

    void BoogieLexer::getSimpleIdentifier() {
        startToken();
        while (isSimpleIdentifierChar(curChar()))
            getTokenChar();

        endToken(rwMap.getOrElse(curTokenText, Token::Kind::identifier));
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
