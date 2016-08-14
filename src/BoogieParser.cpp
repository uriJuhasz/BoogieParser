#include "BoogieParser.h"
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <stack>

#include "BoogieAST.h"
using namespace std;

class BoogieParser final
{
    public:
        BoogieParser(ifstream& _f) :f(_f){}
        ~BoogieParser(){}
        unique_ptr<BoogieProgram> parse();

    protected:

        bool done();
//        typedef void (BoogieParser::*PFunc)();
        void parseGlobalDeclaration();
        void parseTypeDeclaration();
        void parseConstDeclaration();
        void parseFunctionDeclaration();
        void parseAxiom();
        void parseVariableDeclaration();
        void parseProcedureDeclaration();
        void parseImplementationDeclaration();

        bool peekReservedWord(const string&);
        void parseReservedWord(const string &);
        BoogieIdentifier parseIdentifier();
        list<BoogieIdentifier> parseIdentifierList();
        BoogieType parseType();

        BoogieAttributes parseAttributes();
    private:
        ifstream& f;
        string buf;

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
    if (peekReservedWord(typeDeclarationRW))
        parseTypeDeclaration();
    else if (peekReservedWord(constDeclarationRW))
        parseConstDeclaration();
    else if (peekReservedWord(varDeclarationRW))
        parseVariableDeclaration();
    else if (peekReservedWord(functionDeclarationRW))
        parseFunctionDeclaration();
    else if (peekReservedWord(axiomDeclarationRW))
        parseAxiom();
    else if (peekReservedWord(procedureDeclarationRW))
        parseProcedureDeclaration();
    else if (peekReservedWord(implementationDeclarationRW))
        parseImplementationDeclaration();
}

void BoogieParser::parseTypeDeclaration(){
    parseReservedWord(typeDeclarationRW);
    auto attributes = parseAttributes();
    const BoogieIdentifier name = parseIdentifier();
    auto typeParameters = parseIdentifierList();
    auto synonym = (tryParse(equalityRW)) ? parseType() : BoogieNoType::get();
    parseSemiColon();
    program->typeDeclarations.push_back(new BoogieTypeDeclaration(name,attributes,typeParameters,synonym));
}

unique_ptr<BoogieAttributes> BoogieParser::parseAttributes(){
    unique_ptr<BoogieAttributes> r = new BoogieAttributes();
    if (tryParseOpenCurly()){
        while (!tryParseCloseCurly()){
            r->push_back(parseBoogieAttribute());
        }
    }
    return r;
}

class IOException : exception{
    public:IOException(const string _errorMessage) : errorMessage(_errorMessage){}
    const string errorMessage;
};
void parseBoogieFile(const string& bplFileName){
    ifstream f(bplFileName);
    if (!f.is_open())
        throw new IOException("Cannot open file \"" + bplFileName + "\"");

    BoogieParser parser(f);
    parser.parse();
}
