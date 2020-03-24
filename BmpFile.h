#pragma once

typedef unsigned __int8 BYTE;
typedef unsigned __int16 WORD;
typedef unsigned __int32 DWORD;
typedef __int32 LONG;

struct {
	// bfType
	WORD signature;
	// bfOffBits
	DWORD dataOffset;
	// biSize
	DWORD headerVersion;
	// biWidth
	LONG imgWidth;
	// biHeight
	LONG imgHeight;
	// biBitCount
	WORD bitsInPixel;
	// biCompression
	DWORD bitCompressionType;
	// biSizeImage
	DWORD bufferSize;
} BmpFile;