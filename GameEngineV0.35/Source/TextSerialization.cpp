#include "Precompiled.h"
#include "TextSerialization.h"

namespace Framework
{
	bool TextSerializer::Open(const std::string& file)
	{
		stream.open(file.c_str());
		return stream.is_open();
	}

	bool TextSerializer::IsGood()
	{
		return stream.good();
	}

	void TextSerializer::ReadInt(int& i)
	{
		stream >> i;
	}

  void TextSerializer::ReadUnsignedInt( unsigned int &ui )
  {
    stream >> ui;
  }

	void TextSerializer::ReadFloat(float& f)
	{
		stream >> f;
	}

	void TextSerializer::ReadString(std::string& str)
	{
		stream >> str;
	}

  void TextSerializer::ReadLine( std::string &str )
  {
    stream.ignore();
    std::getline( stream, str );
  }

}