#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring> // Include <cstring> for strtok and strcpy
#include "filter.h"
using namespace std;

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xResolution;
    int32_t yResolution;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)


void read(RGB** image, char name[], int N, int M) {
    FILE* frame_c;
    if ((frame_c = fopen(name, "rb")) == NULL) {
        cout << "Image doesn't exist";
        exit(-1);
    }
    
    // Skip the BMP header
    fseek(frame_c, sizeof(BMPHeader), SEEK_SET);
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fread(&image[i][j],sizeof(RGB),1,frame_c);
        }
    }

    fclose(frame_c);
}

void write(RGB** out, char dir[], char outName[], int N, int M,BMPHeader& header) {
    FILE* frame_y;
    char token[200]; // Increase the buffer size
    strcpy(token, dir);
    strcat(token, outName);
    frame_y = fopen(token, "wb");
    
   // Write the BMP header
   fwrite(&header,sizeof(BMPHeader),1,frame_y);
   
   // Write the image data
   for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fwrite(&out[i][j],sizeof(RGB),1,frame_y);
        }
    }
    
   fclose(frame_y);
}

int main() {
    
	// Specify the name of the input image
	char name[] = "input.bmp";

	// Open and read BMP header
	ifstream file(name, ios::binary);
	BMPHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	file.close();
	
	// Get image dimensions from the header
	int N = header.height;
	int M = header.width;
	cout << N << " " << M ;
	
	// Create the image array
	RGB** image = new RGB*[N];
	for (int i = 0; i < N; i++) {
		image[i] = new RGB[M];
	}
	
	// Read the image from the file
	read(image, name, N, M);

	// Specify the output directory (current directory) and file name (without extension)
	char dir[] = "./";
	char outName[] = "output_image.bmp";
    
	// Apply the Sobel filter algorithm
	Sobel_algorithm(N,M, image);
	
	// Write the image to the output file
	write(image, dir, outName, N, M,header);

	for(int i = 0; i < N; i++)
		delete [] image[i];
	
	delete [] image;

	return 0;
}
