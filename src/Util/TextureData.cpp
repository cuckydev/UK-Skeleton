/*
 * UK Skeleton
 * File: Util/TextureData.cpp
 * 
 * Purpose: Define texture data class
 * 
 * Authour(s): Regan "CuckyDev" Green
*/

#include "TextureData.h"
#include <fstream>
#include <cstddef>

#include <lodepng/lodepng.h>

//Internal interface
/*
 * TextureData::Read:
 *   Reads the PNG formatted data from the given stream using lodepng
 * 
 * Arguments:
 *   stream - Stream to read from
 * 
 * Returns:
 *   Status code (false on success, true on failure)
*/
bool TextureData::Read(std::istream &stream)
{
	//Get length of stream
	stream.seekg(0, stream.end);
	size_t size = stream.tellg();
	stream.seekg(0, stream.beg);
	
	//Read data
	char *buffer = new char[size];
	if (buffer == nullptr)
		return error.Push("Failed to allocate stream buffer");
	stream.read(buffer, size);
	
	//Process data as PNG
	unsigned int decode_error;
	if ((decode_error = lodepng_decode32((unsigned char**)&data, &width, &height, (const unsigned char*)buffer, size)) != 0)
		return error.Push(lodepng_error_text(decode_error));
	
	//Delete file buffer
	delete[] buffer;
	
	return false;
}

//Constructors and destructor
/*
 * TextureData::TextureData:
 *   Texture data constructor, reading from a file at the given path
 * 
 * Arguments:
 *   path - File path
*/
TextureData::TextureData(std::string path)
{
	//Read from path as a file
	std::ifstream stream(path, std::ifstream::binary);
	if (!stream)
	{
		error.Push("Failed to open texture file at " + path);
		return;
	}
	Read(stream);
}

/*
 * TextureData::TextureData:
 *   Texture data constructor, reading from the given stream
 * 
 * Arguments:
 *   stream - Stream to read from
*/
TextureData::TextureData(std::istream &stream)
{
	//Read stream
	Read(stream);
}

/*
 * TextureData::~TextureData:
 *   Texture data destructor
*/
TextureData::~TextureData()
{
	//Delete loaded data
	free(data);
}
