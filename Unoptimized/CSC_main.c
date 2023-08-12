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


int main( void) {
  int row, col;
  FILE *f_ID_input_RGB;
  // FILE *f_ID_echo_R;
  // FILE *f_ID_echo_G;
  // FILE *f_ID_echo_B;
  // FILE *f_ID_output_Y;
  // FILE *f_ID_output_Cb;
  // FILE *f_ID_output_Cr;
  FILE *f_ID_output_RGB;


  // Open the BMP file for reading
  f_ID_input_RGB = fopen("../Images/BigImage.bmp", "rb");
  if (f_ID_input_RGB == NULL) {
      printf("Error opening file f_ID_input_RGB.\n");
      return 1;
  }

  // Read the BMP file header
  BITMAPFILEHEADER file_header;
  fread(&file_header, sizeof(BITMAPFILEHEADER), 1, f_ID_input_RGB);

  // Check if the file is in BMP format
  if (file_header.type != 0x4D42) {
      printf("The file is not a BMP image.\n");
      fclose(f_ID_input_RGB);
      return 1;
  }

  // Read the BMP info header
  BITMAPINFOHEADER info_header;
  fread(&info_header, sizeof(BITMAPINFOHEADER), 1, f_ID_input_RGB);

  printf("Width: %d\n", info_header.width);
  printf("Height: %d\n", info_header.height);
  
  // Get the image dimensions
  int width = info_header.width;
  int height = abs(info_header.height); // Height can be negative for top-down BMP

  // f_ID_echo_R = fopen( "./image_echo_R_64_48_03.data", "wb");
  // if( f_ID_echo_R == NULL) {
  //   printf( "Cannot open file.\n");
  //   return( 1);
  // }

  // f_ID_echo_G = fopen( "./image_echo_G_64_48_03.data", "wb");
  // if( f_ID_echo_G == NULL) {
  //   printf( "Cannot open file.\n");
  //   return( 1);
  // }

  // f_ID_echo_B = fopen( "./image_echo_B_64_48_03.data", "wb");
  // if( f_ID_echo_B == NULL) {
  //   printf( "Cannot open file.\n");
  //   return( 1);
  // }

  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col++) {
    R[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
    //fputc( R[row][col], f_ID_echo_R);
//
    G[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
   // fputc( G[row][col], f_ID_echo_G);
//
    B[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
  //  fputc( B[row][col], f_ID_echo_B);
  }
  // fclose( f_ID_echo_B);
  // fclose( f_ID_echo_G);
  // fclose( f_ID_echo_R);
  fclose( f_ID_input_RGB);

  CSC_RGB_to_YCC();

  // f_ID_output_Y = fopen( "./image_output_Y_64_48_03.data", "wb");
  // if( f_ID_output_Y == NULL) {
  //   fprintf( stderr, "Could not open %s\n", 
  //            "./image_output_Y_64_48_03.data");
  //   return( 1);
  // }
  
  // f_ID_output_Cb = fopen( "./image_output_Cb_64_48_03.data", "wb");
  // if( f_ID_output_Cb == NULL) {
  //   fprintf( stderr, "Could not open %s\n", 
  //            "./image_output_Cb_64_48_03.data");
  //   return( 1);
  // }
  
  // f_ID_output_Cr = fopen( "./image_output_Cr_64_48_03.data", "wb");
  // if( f_ID_output_Cr == NULL) {
  //   fprintf( stderr, "Could not open %s\n", 
  //            "./image_output_Cr_64_48_03.data");
  //   return( 1);
  // }
  
  // for( row=0; row < IMAGE_ROW_SIZE; row++)
  // for( col=0; col < IMAGE_COL_SIZE; col++) {
  //   //fprintf( f_ID_output_Y, "%02hhx", Y[row][col]);
  //   fputc( Y[row][col], f_ID_output_Y);
  // }

  // for( row=0; row < (IMAGE_ROW_SIZE >> 1); row++)
  // for( col=0; col < (IMAGE_COL_SIZE >> 1); col++) {
  //   //fprintf( f_ID_output_Cb, "%02hhx", Cb[row][col]);
  //   fputc( Cb[row][col], f_ID_output_Cb);
  //   //fprintf( f_ID_output_Cr, "%02hhx", Cr[row][col]);
  //   fputc( Cr[row][col], f_ID_output_Cr);
  // }

  // fclose( f_ID_output_Cr);
  // fclose( f_ID_output_Cb);
  // fclose( f_ID_output_Y);

  CSC_YCC_to_RGB();


  //Write RGB output to bitmap file
  FILE* outputRGBfile = fopen("./unoptimizedOutputRGB.bmp", "wb");
  if (outputRGBfile == NULL) {
      printf("Error opening file.\n");
      return 1;
  }

  fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, outputRGBfile);
  fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, outputRGBfile);
  fseek (outputRGBfile, file_header.offset, SEEK_SET);

  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col++) {
    fputc( R[row][col], outputRGBfile);
    fputc( G[row][col], outputRGBfile);
    fputc( B[row][col], outputRGBfile);
  }
  fclose( outputRGBfile);

} // END of main()

