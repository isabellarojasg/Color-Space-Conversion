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

//represents a pixel in an image storing in RGB format
typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;  

} RGB;

typedef struct {
	int width;
	int height;
  RGB **rgb;
} Image;


//read image from file and return the Image struct containing file data
Image readImage(FILE *fp, int height, int width){
  Image picture;

  //2D array with height rows, each row is pointer to array of RGB structs
  picture.rgb = (RGB**) malloc (height*sizeof(void*));
  picture.height = height;
  picture.width = width;
  int i;
  for(i= height-1 ; i = 0; i++){
    picture.rgb[i] = (RGB*) malloc(width * sizeof(RGB));
  }
  return picture;
}

void freeImage(Image picture){
  int i;
  for(i= height-1 ; i = 0; i++){
    free(picture.rgb[i]);
    free(picture.rgb);
  }
}

int main( void) {

  
  int row, col;
    // Open the BMP file for reading
    FILE* file = fopen("./Images/newrgb.bmp", "rb");
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

    printf("Size: %u\n", info_header.size);
    printf("Width: %d\n", info_header.width);
    printf("Height: %d\n", info_header.height);
    printf("Bits Per Pixel: %u\n", info_header.bits_per_pixel);
    printf("Compression: %u\n", info_header.compression);
    printf("Image Size: %u\n", info_header.image_size);
    
    // Get the image dimensions
    int width = info_header.width;
    int height = abs(info_header.height); // Height can be negative for top-down BMP

    // Calculate image size (without padding)
    int image_size = width * height * 3; // 3 bytes per pixel for 24-bit BMP
    Image image = readImage(file,height,width);
    fclose(file);
    freeImage(image)
    // // Allocate memory to store pixel data
    // uint8_t* pixel_data = (uint8_t*)malloc(image_size);
    // if (pixel_data == NULL) {
    //     printf("Memory allocation error.\n");
    //     fclose(file);
    //     return 1;
    // }

    // // Read the pixel data from the file
    // fseek(file, file_header.offset, SEEK_SET);
    // fread(pixel_data, sizeof(uint8_t), image_size, file);

    // // Close the file
    // fclose(file);

    // // Separate RGB values
    // uint8_t* red_channel = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    // uint8_t* green_channel = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    // uint8_t* blue_channel = (uint8_t*)malloc(width * height * sizeof(uint8_t));

    // if (red_channel == NULL || green_channel == NULL || blue_channel == NULL) {
    //   printf("Memory allocation error.\n");
    //   free(pixel_data);
    //   free(red_channel);
    //   free(green_channel);
    //   free(blue_channel);
    //   return 1;
    // }

    // int pixel_index = 0;
    // int y;
    // int x;
    // for (y = 0; y < height; y++) {
    //     for (x = 0; x < width; x++) {
    //         // BMP stores pixel data in the order: Blue, Green, Red
    //         blue_channel[pixel_index] = pixel_data[pixel_index * 3];
           
    //         //green_channel[pixel_index] = pixel_data[pixel_index * 3 + 1];
    //         //red_channel[pixel_index] = pixel_data[pixel_index * 3 + 2];
    //         pixel_index++;
    //     }
    // }


    // FILE* blue_file = fopen("blue_channel.bmp", "wb");

    // fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, blue_file);
    // fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, blue_file);

    // fwrite(blue_channel, sizeof(uint8_t), width * height, blue_file);

    // fclose(blue_file);

    // free(pixel_data);
    // free(red_channel);
    // free(green_channel);
    // free(blue_channel);
//   f_ID_echo_R = fopen( "./Images/R.data", "wb");
//   if( f_ID_echo_R == NULL) {
//     printf( "Cannot open file echo_R_64_48_03.\n");
//     return( 1);
//   }

//   f_ID_echo_G = fopen( "./Images/G.data", "wb");
//   if( f_ID_echo_G == NULL) {
//     printf( "Cannot open file echo_G_64_48_03.\n");
//     return( 1);
//   }

//   f_ID_echo_B = fopen( "./Images/B.data", "wb");
//   if( f_ID_echo_B == NULL) {
//     printf( "Cannot open file echo_B_64_48_03.\n");
//     return( 1);
//   }

//   for( row=0; row < IMAGE_ROW_SIZE; row++)
//   for( col=0; col < IMAGE_COL_SIZE; col++) {
//     R[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
//     fputc( R[row][col], f_ID_echo_R);
// //
//     G[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
//     fputc( G[row][col], f_ID_echo_G);
// //
//     B[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
//     fputc( B[row][col], f_ID_echo_B);
//   }
//   fclose( f_ID_echo_B);
//   fclose( f_ID_echo_G);
//   fclose( f_ID_echo_R);

  // CSC_RGB_to_YCC();

  // f_ID_output_Y = fopen( "./Images/Y.data", "wb");
  // if( f_ID_output_Y == NULL) {
  //   fprintf( stderr, "Could not open %s\n", 
  //            "./image_output_Y_64_48_03.data");
  //   return( 1);
  // }
  
  // f_ID_output_Cb = fopen( "./Images/Cb.data", "wb");
  // if( f_ID_output_Cb == NULL) {
  //   fprintf( stderr, "Could not open %s\n", 
  //            "./image_output_Cb_64_48_03.data");
  //   return( 1);
  // }
  
  // f_ID_output_Cr = fopen( "./Images/Cr.data", "wb");
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

  // CSC_YCC_to_RGB();

  // f_ID_output_RGB = fopen( "./Images/OutputRGB.data", "wb");
  // if( f_ID_output_RGB == NULL) {
  //   printf( "Cannot open file RGB_64_48_03.\n");
  //   return( 1);
  // }

  // for( row=0; row < IMAGE_ROW_SIZE; row++)
  // for( col=0; col < IMAGE_COL_SIZE; col++) {
  //   fputc( R[row][col], f_ID_output_RGB);
  //   fputc( G[row][col], f_ID_output_RGB);
  //   fputc( B[row][col], f_ID_output_RGB);
  // }
  // fclose( f_ID_output_RGB);

  //gettimeofday(&end_time, NULL); // Record the end time

} // END of main()

