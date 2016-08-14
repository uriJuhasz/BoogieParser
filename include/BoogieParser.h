#ifndef BOOGIEPARSER_H
#define BOOGIEPARSER_H

#include<string>
#include<memory>
#include "../src/BoogieAST.h"

unique_ptr<BoogieProgram> parseBoogieFile(const std::string& bplFileName);
#endif // BOOGIEPARSER_H
