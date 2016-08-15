#include "BoogieParser.h"
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <stack>
#include <unordered_set>

#include "BoogieAST.h"
using namespace std;

class BoogieParser final
{
    public:
        BoogieParser(ifstream& _f) :f(_f){}
        ~BoogieParser(){}
        unique_ptr<BoogieProgram> parse();

        unique_ptr<BoogieProgram> getProgram(){
            return move(program);
        }
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

        unique_ptr<BoogieIdentifier> parseIdentifier();
        unique_ptr<BoogieTypeParameterList> parseTypeParameterList();
        unique_ptr<BoogieType> parseType();
        unique_ptr<BoogieAttribute> parseBoogieAttribute();

        bool peekReservedWord(const string&);
        void parseReservedWord(const string &);
        void parseSemicolon();
        bool tryParseOpenCurly();
        bool tryParseCloseCurly();
        bool tryParseEqualSign();

        unique_ptr<BoogieAttributes> parseAttributes();

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
    auto name = parseIdentifier();
    auto typeParameters = parseTypeParameterList();
    unique_ptr<BoogieTypeDeclaration> decl;
    if (tryParseEqualSign()){
        auto synonym = parseType();
        decl = make_unique<BoogieTypeSynonymDeclaration>(name,attributes,typeParameters,synonym);
    }else
        decl = make_unique<BoogieTypeConstructorDeclaration>(name,attributes,typeParameters);
    parseSemicolon();
    program->addTypeDeclaration(move(decl));
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
