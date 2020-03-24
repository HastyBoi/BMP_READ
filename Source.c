#include <stdlib.h>
#include <stdio.h>
#include "BmpFile.h"

#define BMP_SIGNATURE 0x4d42
#define BMP_DATA_OFFSET_POSITION 0x0a
#define BMP_INFO_HEADER_VERSION 40
#define BMP_COMPRESSION_TYPE 0
#define READ_ONE_BYTE 1

int readBit(const int number, const int target) {
	return number & (1 << target);
}

void readFromFileToBuffer(char const fileName[], BYTE** buffer) {

	FILE* inputFile;

	if (fopen_s(&inputFile, fileName, "r")) {
		fprintf_s(stderr, "Can't open image\n");
		exit(EXIT_FAILURE);
	}

	fread_s(&BmpFile.signature, sizeof(BmpFile.signature), READ_ONE_BYTE, sizeof(WORD), inputFile);
	if (BmpFile.signature != BMP_SIGNATURE) {
		fprintf_s(stderr, "Wrong image type\n");
		exit(EXIT_FAILURE);
	}

	fseek(inputFile, BMP_DATA_OFFSET_POSITION, SEEK_SET);
	fread_s(&BmpFile.dataOffset, sizeof(BmpFile.dataOffset), READ_ONE_BYTE, sizeof(DWORD), inputFile);

	fread_s(&BmpFile.headerVersion, sizeof(BmpFile.headerVersion), READ_ONE_BYTE, sizeof(DWORD), inputFile);
	if (BmpFile.headerVersion != BMP_INFO_HEADER_VERSION) {
		fprintf_s(stderr, "Wrong bmp version\n");
		exit(EXIT_FAILURE);
	}

	fread_s(&BmpFile.imgWidth, sizeof(BmpFile.imgWidth), READ_ONE_BYTE, sizeof(LONG), inputFile);
	fread_s(&BmpFile.imgHeight, sizeof(BmpFile.imgHeight), READ_ONE_BYTE, sizeof(LONG), inputFile);
	if (BmpFile.imgHeight >= 0) {
		fprintf_s(stderr, "Wrong bmp row order\n");
		exit(EXIT_FAILURE);
	}

	fseek(inputFile, sizeof(WORD), SEEK_CUR);
	fread_s(&BmpFile.bitsInPixel, sizeof(BmpFile.bitsInPixel), READ_ONE_BYTE, sizeof(WORD), inputFile);
	fread_s(&BmpFile.bitCompressionType, sizeof(BmpFile.bitCompressionType), READ_ONE_BYTE, sizeof(DWORD), inputFile);
	if (BmpFile.bitCompressionType != BMP_COMPRESSION_TYPE) {
		fprintf_s(stderr, "Wrong bmp compression type\n");
		exit(EXIT_FAILURE);
	}

	fread_s(&BmpFile.bufferSize, sizeof(BmpFile.bufferSize), READ_ONE_BYTE, sizeof(DWORD), inputFile);

	*buffer = (BYTE*)malloc(BmpFile.bufferSize * sizeof(BYTE));

	fseek(inputFile, BmpFile.dataOffset, SEEK_SET);
	fread_s(*buffer, BmpFile.bufferSize, READ_ONE_BYTE, BmpFile.bufferSize, inputFile);

	fclose(inputFile);
}

void writeToFile(BYTE* buffer, char const fileName[]) {
	char const fillSymbol = '#';
	char const emptySymbol = ' ';
	FILE* outputFile;

	if (fileName) {
		if (fopen_s(&outputFile, fileName, "w")) {
			fprintf_s(stderr, "Can't open image\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		outputFile = stdout;

	for (int i = 0; i < BmpFile.bufferSize; ++i) {
		for (int j = 7; j >= 0; --j)
			if (readBit(buffer[i], j))
				fprintf_s(outputFile, "%c", fillSymbol);
			else
				fprintf_s(outputFile, "%c", emptySymbol);
		if ((i + 1) % 4 == 0)
			fprintf_s(outputFile, "\n");
	}
	fprintf_s(outputFile, "\n");
	fclose(outputFile);
}

int main() {
	char const imgName[] = "img.bmp";

	BYTE* buff;

	readFromFileToBuffer(imgName, &buff);
	writeToFile(buff, NULL);

	free(buff);


	return EXIT_SUCCESS;
}