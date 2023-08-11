// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// main() function

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <string.h>
#include <math.h>

#define GLOBAL
#include "CSC_global.h"

#pragma pack(push, 1)

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t total_colors;
    uint32_t important_colors;
} BITMAPINFOHEADER;
#pragma pack(pop)


//read image from file and return the Image struct containing image data
Image readImage(FILE *fp, int height, int width){
  Image pic;

  pic.height = height;
  pic.width = width;

  //allocate memory for 1D array to store all image pixel data
  //pic.pixels is pointer to 1D array of Pixel structures
  pic.pixels = (Pixel*) malloc (sizeof(Pixel) * pic.height * pic.width);

  
  return pic;
}

// void freeImage(Image picture){
//   int i;
//   for(i= height-1 ; i = 0; i++){
//     free(picture.rgb[i]);
//     free(picture.rgb);
//   }
// }

int main( void) {

  
  int row, col;
    // Open the BMP file for reading
    FILE* file = fopen("../Images/new.bmp", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read the BMP file header
    BITMAPFILEHEADER file_header;
    fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file);

    // Check if the file is in BMP format
    if (file_header.type != 0x4D42) {
        printf("The file is not a BMP image.\n");
        fclose(file);
        return 1;
    }

    // Read the BMP info header
    BITMAPINFOHEADER info_header;
    fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

    // printf("Size: %u\n", info_header.size);
    printf("Width: %d\n", info_header.width);
    printf("Height: %d\n", info_header.height);
    // printf("Bits Per Pixel: %u\n", info_header.bits_per_pixel);
    // printf("Compression: %u\n", info_header.compression);
    // printf("Image Size: %u\n", info_header.image_size);
    
    // Get the image dimensions
    int width = info_header.width;
    int height = abs(info_header.height); // Height can be negative for top-down BMP

   // Calculate padding (if any) at the end of each row in BMP

    Image pic;
    pic.height = height;
    pic.width = width;

    pic.pixels = (Pixel*) malloc (sizeof(Pixel) * pic.height * pic.width);

    int padding = (4 - (width * sizeof(Pixel)) % 4) % 4;
    printf("padding: %d\n", padding);

        // Get the data offset
    uint32_t dataOffset = file_header.offset;

    printf("Data Offset: %u\n", dataOffset);
    // Move to the beginning of pixel data
    fseek(file, dataOffset, SEEK_SET);
    fread(pic.pixels, sizeof(Pixel), pic.height * pic.width, file);

   // Image image = readImage(file, height, width);
    fclose(file);

    // FILE* outputFile = fopen("output.txt", "wb");
    // int i;
    // // Write the pixel data to the output file
    // for (i = 0; i < pic.height * pic.width; i++) {
    //     fprintf(outputFile, "Pixel %d: R=%d, G=%d, B=%d\n", i + 1,
    //             pic.pixels[i].r, pic.pixels[i].g, pic.pixels[i].b);
    // }
    // fclose(outputFile);

    CSC_RGB_to_YCC(pic);


    free(pic.pixels);

} // END of main()

