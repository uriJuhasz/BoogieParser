#include "../include/BoogieParser.h"
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>
#include <locale>
#include <tuple>
#include <sstream>

#include "BoogieAST.h"
using namespace std;

class Position final{
public:
    typedef size_t Size;
    Position(Size _line,Size _col) : line(_line),col(_col){}
    Size line;
    Size col;
    string toString() const{
    	stringstream s;
    	s << line + ":" + col;
    	return s.str();
    }
};
class ParserBase {
public:
    class ParserException : public exception{
    	public:
    		ParserException(const Position _pos) : pos(_pos){};
    		const Position pos;
    		virtual string message();

    };
    class ReadPastEOFException : public ParserException{
    	public:
    		ReadPastEOFException(const Position _pos) : ParserException(_pos){}
    		virtual string message(){return "Read past EOF at " + pos.toString();}
    };
    class StreamReadException : public ParserException{public:StreamReadException(const Position _pos) : ParserException(_pos){}};
    class ParserMismatchException : public ParserException{
    	public:
    		ParserMismatchException(const Position _pos,const string& _expected) : ParserException(_pos), expected(_expected){}
    		const string expected;
    		virtual string message(){return "Syntax error at " + pos.toString() + " expected \"" + expected + "\"";}
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

class BoogieParser final : public ParserBase
{
    public:
        BoogieParser(istream& _f) :ParserBase(_f){}
        ~BoogieParser(){}
        unique_ptr<BoogieProgram> parse();

        unique_ptr<BoogieProgram> getProgram(){
            return move(program);
        }
    protected:

//        typedef void (BoogieParser::*PFunc)();
        void parseGlobalDeclaration();
        void parseTypeDeclaration();
        void parseConstDeclaration();
        void parseFunctionDeclaration();
        void parseAxiom();
        void parseVariableDeclaration();
        void parseProcedureDeclaration();
        void parseImplementationDeclaration();

        unique_ptr<BoogieIdentifier> parseIdentifier();
        unique_ptr<BoogieTypeParameterList> parseTypeParameterList();
        unique_ptr<BoogieType> parseType();
        unique_ptr<BoogieAttribute> parseBoogieAttribute();

        void parseReservedWord(const string &);
        bool tryParseReservedWord(const string &);
        void parseSemicolon();
        bool tryParseOpenCurly();
        bool tryParseCloseCurly();
        bool tryParseEqualSign();

        unique_ptr<BoogieAttributes> parseAttributes();

    private:

        unique_ptr<BoogieProgram> program;
        stack<BoogieScope*> scopeStack;
        BoogieScope& currentScope(){return *scopeStack.top();};

        const string typeDeclarationRW = "type";
        const string varDeclarationRW = "var";
        const string constDeclarationRW = "const";
        const string functionDeclarationRW = "function";
        const string axiomDeclarationRW = "axiom";
        const string procedureDeclarationRW = "procedure";
        const string implementationDeclarationRW = "implementation";

        const unordered_set<string> reservedWords{
            typeDeclarationRW,
            varDeclarationRW,
            constDeclarationRW,
            functionDeclarationRW,
            axiomDeclarationRW,
            procedureDeclarationRW,
            implementationDeclarationRW
        };
};

unique_ptr<BoogieProgram> BoogieParser::parse(){
    program = make_unique<BoogieProgram>();
    scopeStack.push(program.get());
    while (!done()){
        parseGlobalDeclaration();
    }
    return move(program);
/*    string s;
    while (!f.eof()){
        getline(f,s);
        cout << s << endl;
    }*/
}

void BoogieParser::parseGlobalDeclaration(){
    if (tryParseString(typeDeclarationRW))
        parseTypeDeclaration();
    else if (tryParseString(constDeclarationRW))
        parseConstDeclaration();
    else if (tryParseString(varDeclarationRW))
        parseVariableDeclaration();
    else if (tryParseString(functionDeclarationRW))
        parseFunctionDeclaration();
    else if (tryParseString(axiomDeclarationRW))
        parseAxiom();
    else if (tryParseString(procedureDeclarationRW))
        parseProcedureDeclaration();
    else if (tryParseString(implementationDeclarationRW))
        parseImplementationDeclaration();
}

void BoogieParser::parseTypeDeclaration(){
    auto attributes = parseAttributes();
    auto name = parseIdentifier();
    auto typeParameters = parseTypeParameterList();
    unique_ptr<BoogieTypeDeclaration> decl;
    if (tryParseEqualSign()){
        auto synonym = parseType();
        decl = make_unique<BoogieTypeSynonymDeclaration>(name,attributes,typeParameters,synonym);
    }else
        decl = make_unique<BoogieTypeConstructorDeclaration>(name,attributes,typeParameters);
    parseSemicolon();
    program->addTypeDeclaration(decl);
}

unique_ptr<BoogieAttributes> BoogieParser::parseAttributes(){
    auto r = make_unique<BoogieAttributes>();
    if (tryParseOpenCurly()){
        while (!tryParseCloseCurly()){
            r->attributes.push_back(parseBoogieAttribute());
        }
    }
    return r;
}

class IOException : exception{
    public:IOException(const string _errorMessage) : errorMessage(_errorMessage){}
    const string errorMessage;
};
unique_ptr<BoogieProgram> parseBoogieFile(const string& bplFileName){
    ifstream f(bplFileName);
    if (!f.is_open())
        throw new IOException("Cannot open file \"" + bplFileName + "\"");

    BoogieParser parser(f);
    parser.parse();
    return parser.getProgram();
}
