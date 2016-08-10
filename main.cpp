#include <iostream>
#include <vector>
#include "BoogieParser.h"

using namespace std;

int main(int argc,char*argv[])
{
    cout << "Start" << endl;
    vector<string> args(argv,argv+argc);
//    for (int i=0;i<argc;i++)
//        args.push_back(argv[i]);

    cout << "Arg0:" << args[0] << endl;

    string bplFileName(R"(/home/uri/Downloads/boogie-master/Test/test2/FormulaTerm.bpl)");
    cout << "Parsing " << bplFileName << endl;
    parseBoogieFile(bplFileName);

    cout << "End" << endl;
    return 0;
}
