#include "BoogieParser.h"
#include <istream>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>
using namespace std;

class BoogieParser final
{
    public:
        BoogieParser(ifstream& _f) :f(_f){}
        ~BoogieParser(){}
        void parse();

    private:
        ifstream& f;
};

void BoogieParser::parse(){
    string s;
    while (!f.eof()){
        getline(f,s);
        cout << s << endl;
    }
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
