/*
 * Position.h
 *
 *  Created on: Aug 17, 2016
 *      Author: uri
 */

#ifndef INCLUDE_POSITION_H_
#define INCLUDE_POSITION_H_

#include <string>
#include <sstream>

namespace BoogieParser {
	class Position final{
	public:
		typedef size_t Size;
		Position();
		Position(Size _line,Size _col);
		Size line;
		Size col;
		std::string toString() const;
	};
} /* namespace BoogieParser */

#endif /* INCLUDE_POSITION_H_ */
