/*
 * Position.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: uri
 */

#include "Position.h"
#include <sstream>
using namespace std;

namespace BoogieParser {
	Position::Position() : line(-1),col(-1){}
	Position::Position(Size _line,Size _col) : line(_line),col(_col){}
	string Position::toString() const{
		stringstream s;
		s << line + ":" + col;
		return s.str();
	}
} /* namespace BoogieParser */
