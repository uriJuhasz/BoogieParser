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
#include <tuple>
#include <sstream>

#include "BoogieAST.h"
#include "Position.h"
using namespace std;
using namespace BoogieAST;

namespace BoogieParser{
	/*
	 * 			case 1:
					if (ch >= '#' && ch <= '$' || ch == 39 || ch == '.' || ch == '?' || ch >= 'A' && ch <= 'Z' || ch >= '^' && ch <= 'z' || ch == '~') {AddCh(); goto case 2;}
					else {goto case 0;}
				case 2:
					recEnd = pos; recKind = 1;
					if (ch >= '#' && ch <= '$' || ch == 39 || ch == '.' || ch >= '0' && ch <= '9' || ch == '?' || ch >= 'A' && ch <= 'Z' || ch >= '^' && ch <= 'z' || ch == '~') {AddCh(); goto case 2;}
					else {t.kind = 1; t.val = new String(tval, 0, tlen); CheckLiteral(); return t;}
	 *
	 */



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

	bool BoogieParser::tryParseOpenCurly(){
		return !done() && peekChar()=='{';
	}

	unique_ptr<BoogieIdentifier> BoogieParser::parseIdentifier(){
		auto startPos = curPos;

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
				r->attributes.push_back(move(parseBoogieAttribute()));
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
}//namespace
