// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// RGB to YCC conversion

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "CSC_global.h"

// private data

// =======
static void CSC_RGB_to_YCC_brute_force_int( int row, int col);

// =======
static uint8_t chrominance_downsample(
    uint8_t C_pixel_1, uint8_t C_pixel_2,
    uint8_t C_pixel_3, uint8_t C_pixel_4);

// =======
static void CSC_RGB_to_YCC_brute_force_int( int row, int col) {
//
  int R_pixel_00, R_pixel_01, R_pixel_10, R_pixel_11;
  int G_pixel_00, G_pixel_01, G_pixel_10, G_pixel_11;
  int B_pixel_00, B_pixel_01, B_pixel_10, B_pixel_11;

  int  Y_pixel_00,  Y_pixel_01,  Y_pixel_10,  Y_pixel_11;
  int Cb_pixel_00, Cb_pixel_01, Cb_pixel_10, Cb_pixel_11;
  int Cr_pixel_00, Cr_pixel_01, Cr_pixel_10, Cr_pixel_11;

  R_pixel_00 = (int)R[row+0][col+0];
  R_pixel_01 = (int)R[row+0][col+1];
  R_pixel_10 = (int)R[row+1][col+0];
  R_pixel_11 = (int)R[row+1][col+1];

  G_pixel_00 = (int)G[row+0][col+0];
  G_pixel_01 = (int)G[row+0][col+1];
  G_pixel_10 = (int)G[row+1][col+0];
  G_pixel_11 = (int)G[row+1][col+1];

  B_pixel_00 = (int)B[row+0][col+0];
  B_pixel_01 = (int)B[row+0][col+1];
  B_pixel_10 = (int)B[row+1][col+0];
  B_pixel_11 = (int)B[row+1][col+1];

  Y_pixel_00 = (16 << (K)) + C11 * R_pixel_00
                           + C12 * G_pixel_00
                           + C13 * B_pixel_00;
  Y_pixel_00 += (1 << (K-1)); // rounding
  Y_pixel_00 = Y_pixel_00 >> K;

  Y_pixel_01 = (16 << (K)) + C11 * R_pixel_01
                           + C12 * G_pixel_01
                           + C13 * B_pixel_01;
  Y_pixel_01 += (1 << (K-1)); // rounding
  Y_pixel_01 = Y_pixel_01 >> K;

  Y_pixel_10 = (16 << (K)) + C11 * R_pixel_10
                           + C12 * G_pixel_10
                           + C13 * B_pixel_10;
  Y_pixel_10 += (1 << (K-1)); // rounding
  Y_pixel_10 = Y_pixel_10 >> K;

  Y_pixel_11 = (16 << (K)) + C11 * R_pixel_11
                           + C12 * G_pixel_11
                           + C13 * B_pixel_11;
  Y_pixel_11 += (1 << (K-1)); // rounding
  Y_pixel_11 = Y_pixel_11 >> K;

  Y[row+0][col+0] = (uint8_t)Y_pixel_00;
  Y[row+0][col+1] = (uint8_t)Y_pixel_01;
  Y[row+1][col+0] = (uint8_t)Y_pixel_10;
  Y[row+1][col+1] = (uint8_t)Y_pixel_11;

  Cb_pixel_00 = (128 << (K)) - C21 * R_pixel_00
                             - C22 * G_pixel_00
                             + C23 * B_pixel_00;
  Cb_pixel_00 += (1 << (K-1)); // rounding
  Cb_pixel_00 = Cb_pixel_00 >> K;

  Cb_pixel_01 = (128 << (K)) - C21 * R_pixel_01
                             - C22 * G_pixel_01
                             + C23 * B_pixel_01;
  Cb_pixel_01 += (1 << (K-1)); // rounding
  Cb_pixel_01 = Cb_pixel_01 >> K;

  Cb_pixel_10 = (128 << (K)) - C21 * R_pixel_10
                             - C22 * G_pixel_10
                             + C23 * B_pixel_10;
  Cb_pixel_10 += (1 << (K-1)); // rounding
  Cb_pixel_10 = Cb_pixel_10 >> K;

  Cb_pixel_11 = (128 << (K)) - C21 * R_pixel_11
                             - C22 * G_pixel_11
                             + C23 * B_pixel_11;
  Cb_pixel_11 += (1 << (K-1)); // rounding
  Cb_pixel_11 = Cb_pixel_11 >> K;

  Cr_pixel_00 = (128 << (K)) + C31 * R_pixel_00
                             - C32 * G_pixel_00
                             - C33 * B_pixel_00;
  Cr_pixel_00 += (1 << (K-1)); // rounding
  Cr_pixel_00 = Cr_pixel_00 >> K;

  Cr_pixel_01 = (128 << (K)) + C31 * R_pixel_01
                             - C32 * G_pixel_01
                             - C33 * B_pixel_01;
  Cr_pixel_01 += (1 << (K-1)); // rounding
  Cr_pixel_01 = Cr_pixel_01 >> K;

  Cr_pixel_10 = (128 << (K)) + C31 * R_pixel_10
                             - C32 * G_pixel_10
                             - C33 * B_pixel_10;
  Cr_pixel_10 += (1 << (K-1)); // rounding
  Cr_pixel_10 = Cr_pixel_10 >> K;

  Cr_pixel_11 = (128 << (K)) + C31 * R_pixel_11
                             - C32 * G_pixel_11
                             - C33 * B_pixel_11;
  Cr_pixel_11 += (1 << (K-1)); // rounding
  Cr_pixel_11 = Cr_pixel_11 >> K;

  Cb[row>>1][col>>1] = chrominance_downsample( (uint8_t)Cb_pixel_00,
                                               (uint8_t)Cb_pixel_01,
                                               (uint8_t)Cb_pixel_10,
                                               (uint8_t)Cb_pixel_11);

  Cr[row>>1][col>>1] = chrominance_downsample( (uint8_t)Cr_pixel_00,
                                               (uint8_t)Cr_pixel_01,
                                               (uint8_t)Cr_pixel_10,
                                               (uint8_t)Cr_pixel_11);
} // END of CSC_RGB_to_YCC_brute_force_int()

// =======
static uint8_t chrominance_downsample(
    uint8_t C_pixel_00, uint8_t C_pixel_01,
    uint8_t C_pixel_10, uint8_t C_pixel_11) {

  int temp;

  switch (CHROMINANCE_DOWNSAMPLING_MODE) {
    case 0:
      return( 0);
    case 1:
      return( C_pixel_00);
    case 2:
      temp = (int)C_pixel_00 + (int)C_pixel_01 + 
             (int)C_pixel_10 + (int)C_pixel_11;
      temp += (1 << 1); // rounding
      temp = temp >> 2;
      return( (uint8_t)temp);
    default:
      return( 0);
  }
} // END of chrominance_downsample()

// =======
void CSC_RGB_to_YCC(Image pic) {
  int row, col; // indices for row and column
//
  // for( row=0; row<pic.width * pic.height; row+=2) {
    // for( col=0; col<IMAGE_COL_SIZE; col+=2) { 
      //printf( "\n[row,col] = [%02i,%02i]\n\n", row, col);
      
  yccImage Y;
  Y.width = pic.width; 
  Y.height =  pic.height;
  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * pic.height * pic.width);


  int i;
  for(i = 0; i< pic.height * pic.width; i+=2){
  Y.pixels[i].y = 16 + ((6391*pic.pixels[i].b
                + 32909*pic.pixels[i].g
                +  16763*pic.pixels[i].r)>>16);
 
  Y.pixels[i+1].y = 16 + ((6391*pic.pixels[i+1].b
                + 32909*pic.pixels[i+1].g
                +  16763*pic.pixels[i+1].r)>>16);                     


  Y.pixels[i].cb = 128.0 + ((28672*pic.pixels[i].b
                                   - 18996*pic.pixels[i].g
                                   - 9676*pic.pixels[i].r)>>16);

  Y.pixels[i+1].cb = 128.0 + ((28672*pic.pixels[i+1].b
                                   - 18996*pic.pixels[i+1].g
                                   - 9676*pic.pixels[i+1].r)>>16);

  Y.pixels[i].cr = 128.0 +((- 4662*pic.pixels[i].b
                                   - 24009*pic.pixels[i].g
                                   + 28672*pic.pixels[i].r)>>16);
  Y.pixels[i+1].cr = 128.0 +((- 4662*pic.pixels[i+1].b
                                   - 24009*pic.pixels[i+1].g
                                   + 28672*pic.pixels[i+1].r)>>16); 


  }
    printf("Pixel[%d]: y = %d, cb = %d, cr = %d\n", i, Y.pixels[0].y, Y.pixels[0].cb, Y.pixels[0].cr);
    printf("Pixel[%d]: y = %d, cb = %d, cr = %d\n", i, Y.pixels[962].y, Y.pixels[962].cb, Y.pixels[962].cr);
    printf("Pixel[%d]: y = %d, cb = %d, cr = %d\n", i, Y.pixels[2870].y, Y.pixels[2870].cb, Y.pixels[2870].cr);

    // Open the output file for writing
    // FILE* output_file = fopen("outputYCC.txt", "wb");
    // if (output_file == NULL) {
    //     perror("Error opening output file");
    // }

    // // Write the YCC pixel components to the output file
    // int j;
    // for (j = 0; j < Y.width * Y.height; j++) {
    //         fprintf(output_file, "Pixel %d: Y=%d, Cb=%d, Cr=%d\n", j + 1,
    //           Y.pixels[j].y, Y.pixels[j].cb, Y.pixels[j].cr);
    // }

    // // Close the output file
    // fclose(output_file);

    // Free allocated memory
    free(Y.pixels);
} // END of CSC_RGB_to_YCC()

