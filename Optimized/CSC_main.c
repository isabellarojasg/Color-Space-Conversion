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

Image CSC_RGB_to_YCC(Image pic) {
    int R_pixel_00, R_pixel_01, R_pixel_10, R_pixel_11;
  int G_pixel_00, G_pixel_01, G_pixel_10, G_pixel_11;
  int B_pixel_00, B_pixel_01, B_pixel_10, B_pixel_11;

  int  Y_pixel_00,  Y_pixel_01,  Y_pixel_10,  Y_pixel_11;
  int Cb_pixel_00, Cb_pixel_01, Cb_pixel_10, Cb_pixel_11;
  int Cr_pixel_00, Cr_pixel_01, Cr_pixel_10, Cr_pixel_11;    
  yccImage Y;
  Y.width = pic.width; 
  Y.height =  pic.height;
  int size = pic.width * pic.height;
  int width = pic.width;
  int height = pic.height;

  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * pic.height * pic.width);

  int i;
  //Can probably loop unroll up to 4 in parallel
  for(i = 0; i< pic.height * pic.width; i+=2){
    Y.pixels[i].y = 16 + ((25*pic.pixels[i].b
                  + 129*pic.pixels[i].g
                  +  66*pic.pixels[i].r)>>8);
  
    Y.pixels[i+1].y = 16 + ((25*pic.pixels[i+1].b
                  + 129*pic.pixels[i].g
                  +  66*pic.pixels[i].r)>>8);                    

    Y.pixels[i].cb = 128.0 + ((112*pic.pixels[i].b
                                    - 74*pic.pixels[i].g
                                    - 38*pic.pixels[i].r)>>8);

    Y.pixels[i+1].cb = 128.0 + ((112*pic.pixels[i+1].b
                                    - 74*pic.pixels[i+1].g
                                    - 38*pic.pixels[i+1].r)>>8);

    Y.pixels[i].cr = 128.0 +((- 18*pic.pixels[i].b
                                    - 94*pic.pixels[i].g
                                    + 112*pic.pixels[i].r)>>8);
    Y.pixels[i+1].cr = 128.0 +((- 18*pic.pixels[i+1].b
                                    - 94*pic.pixels[i+1].g
                                    + 112*pic.pixels[i+1].r)>>8);
  }

    FILE* output_file = fopen("outputYCC.txt", "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
    }
  for(i =0; i < size; i++){
  fprintf(output_file, "Pixel %d: Y=%d, Cb=%d, Cr=%d\n", i,
              Y.pixels[i].y, Y.pixels[i].cb, Y.pixels[i].cr);
  }
    // Close the output file
    fclose(output_file);

  //YCC to RGB 
  Image yccRGB;
  yccRGB.height = height;
  yccRGB.width = width;

  yccRGB.pixels = (Pixel*) malloc (sizeof(Pixel) * yccRGB.height * yccRGB.width);

  for(i = 0; i< size; i++){

    int y1 = 74*(Y.pixels[i].y -16);
    int r1 = (y1 + 102*(Y.pixels[i].cr - 128)) >> 6;
    int g1 = ((y1 - 52*(Y.pixels[i].cr - 128) - 25*(Y.pixels[i].cb - 128))) >> 6;
    int b1 = (y1 + 129*(Y.pixels[i].cb - 128)) >> 6;

    yccRGB.pixels[i].r = r1 > 255 ? 255 : (r1 < 0 ? 0 : r1);
    yccRGB.pixels[i].g = g1 > 255 ? 255 : (g1 < 0 ? 0 : g1);
    yccRGB.pixels[i].b = b1 > 255 ? 255 : (b1 < 0 ? 0 : b1);
  }

  return yccRGB;
    // FILE* outputFile = fopen("outputRGB.txt", "wb");
    
    // // Write the pixel data to the output file
    // for (i = 0; i < size; i++) {
    //     // fprintf(outputFile, "Pixel %d: R=%d, G=%d, B=%d\n", i + 1,
    //     //         yccRGB.pixels[i].r, yccRGB.pixels[i].g, yccRGB.pixels[i].b);
    //           fprintf(outputFile, "Pixel %d:  B=%d,  G=%d, R=%d\n", i + 1,
    //              yccRGB.pixels[i].b, yccRGB.pixels[i].g, yccRGB.pixels[i].r);
        
    // }
    // fclose(outputFile);
  // Perform Downsampling
  // yccDSPImage yDSP;
  // yDSP.pixels = (yccDSPPixel *)malloc(sizeof(yccDSPPixel) * size >> 2);
  // int k=0;
  // int j;
  // for(i = 0; i < Y.height >> 1; i++){
  //   for(j = 0; j < Y.width >> 1; j++){
  //     int index = i * 2 * width + j * 2;
  //     yDSP.pixels[k].y1 = Y.pixels[index].y;
  //     yDSP.pixels[k].y2 = Y.pixels[index+1].y;
  //     yDSP.pixels[k].y3 = Y.pixels[index+width].y;
  //     yDSP.pixels[k].y4 = Y.pixels[index+width+1].y;
  //     yDSP.pixels[k].cb = (Y.pixels[index].cb + Y.pixels[index+1].cb + Y.pixels[index+width].cb + Y.pixels[index+width+1].cb) >> 2;
  //     yDSP.pixels[k].cr = (Y.pixels[index].cr + Y.pixels[index+1].cr + Y.pixels[index+width].cr + Y.pixels[index+width+1].cr) >> 2;
  //     k++;
  //   }
  // }
    // // //Open the output file for writing
    // FILE* output_file = fopen("outputDownsampled.txt", "wb");
    // if (output_file == NULL) {
    //     perror("Error opening output file");
    // }

    // // Write the YCC pixel components to the output file
    // for (j = 0; j < k; j++) {
    //         fprintf(output_file, "Pixel %d: y1=%d, y2=%d,y3=%d,y4=%d,cb=%d,cr=%d,\n", j + 1,
    //          yDSP.pixels[j].y1,  yDSP.pixels[j].y2,  yDSP.pixels[j].y3,  yDSP.pixels[j].y4, yDSP.pixels[j].cb, yDSP.pixels[j].cr);
    // }

    // // Close the output file
    // fclose(output_file);

    // Free allocated memory
    
    
    
    //free(yDSP.pixels);

    free(Y.pixels);
} // END of CSC_RGB_to_YCC()


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

    Image outputImage = CSC_RGB_to_YCC(pic);


    free(pic.pixels);

} // END of main()

