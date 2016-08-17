#include "ParserBase.h"
#include <tuple>

namespace BoogieParser {

	ParserBase::ParserException::ParserException(const Position _pos)
		: pos(_pos){}
	ParserBase::ReadPastEOFException::ReadPastEOFException(const Position _pos)
		: ParserException(_pos){}
	std::string ParserBase::ReadPastEOFException::message(){
		return "Read past EOF at " + pos.toString();
	}

	ParserBase::StreamReadException::StreamReadException(const Position _pos)
	: ParserException(_pos){}


	ParserBase::ParserBase(std::istream& _f)
	:f(_f)
	{
		bufPos = 0;
	}
	ParserBase::~ParserBase()
	{
	}

	bool ParserBase::done() const{
		return bufDone() && f.eof();
	}

	ParserBase::Char ParserBase::getChar(){
		if (done())
			throw new ReadPastEOFException(curPos());
		Char c = curChar();
		bufPos++;
		if (isEOL(c))
			_curPos = Position(curPos().line+1,0);
		else
			_curPos = Position(curPos().line,curPos().col+1);
		return c;
	}

	bool ParserBase::isEOL(Char c){return c=='\n';}

	ParserBase::Char ParserBase::curChar(){
		if (bufDone())
			readToBuf();
		if (done())
			throw new ReadPastEOFException(curPos());
		Char c = buf[bufPos];
		return c;
	}

	class ParserBase::CPos{
	public:
		CPos(const Position _pos,Size _bufPos)
			: pos(_pos), bufPos(_bufPos)
		{}
		const Position pos;
		const ParserBase::Size bufPos;
	};

	Position ParserBase::curPos() const
	{
		return _curPos;
	}

	void ParserBase::pushPos()
	{
		posStack.push(CPos(curPos(),bufPos));
	}
	void ParserBase::popPos()
	{
		CPos t = posStack.top();
		posStack.pop();
		bufPos = t.bufPos;
		_curPos = t.pos;
	}
	void ParserBase::discardPopPos(){
		posStack.pop();
	}

	bool ParserBase::bufDone() const{
		return bufPos==static_cast<Size>(buf.size());
	}
	void ParserBase::readToBuf(){
		while (!f.eof() && !f.good()){
			Position pos = curPos();
			Char c = static_cast<Char>(f.get());
			if (!f.good())
				throw new StreamReadException(pos);
			buf.push_back(c);
		}
	}
} /* namespace BoogieParser */
