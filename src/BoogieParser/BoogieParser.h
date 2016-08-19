#ifndef BOOGIEPARSER_H
#define BOOGIEPARSER_H

#include<string>
#include<memory>
#include "../BoogieAST/BoogieAST.h"

namespace BoogieParser{
	std::unique_ptr<BoogieAST::BoogieProgram> parseBoogieFile(const std::string& bplFileName);
}//namespace
#endif // BOOGIEPARSER_H
