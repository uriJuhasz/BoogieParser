#include "BoogieParser.h"
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

#include "BoogieAST.h"
using namespace std;

class Position final{
public:
    typedef size_t Size;
    Position(Size _line,Size _col) : line(_line),col(_col){}
    const Size line,col;
};
class ParserBase {
public:
    class ParserException : public exception{};
    class ReadPastEOFException : public ParserException{};
    class StreamReadException : public ParserException{};
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
            throw new ReadPastEOFException();
        Char c = buf[bufPos];
        return c;
    }

    void parseString(const string& s)
    {

    }

//    class
private:
    bool isEOL(Char c){
    	return c=='\n';
    }
    bool isWhiteSpace(Char c){
    	return isw
    }
    typedef tuple<Size,Position> CPos;
    void pushPos()
    {
        posStack.push(tuple<Size,Position>(bufPos,curPos));
    }
    void popPos()
    {
        auto t = posStack.pop();
        bufPos = t.get(0);
        curPos = t.get(1);
    }
    void discardPopPos(){
        posStack.pop();
    }

    Position curPos(0,0);

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
            int c = f.get();
            if (!f.good())
                throw new StreamReadException();
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
    if (tryParseReservedWord(typeDeclarationRW))
        parseTypeDeclaration();
    else if (tryParseReservedWord(constDeclarationRW))
        parseConstDeclaration();
    else if (tryParseReservedWord(varDeclarationRW))
        parseVariableDeclaration();
    else if (tryParseReservedWord(functionDeclarationRW))
        parseFunctionDeclaration();
    else if (tryParseReservedWord(axiomDeclarationRW))
        parseAxiom();
    else if (tryParseReservedWord(procedureDeclarationRW))
        parseProcedureDeclaration();
    else if (tryParseReservedWord(implementationDeclarationRW))
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
