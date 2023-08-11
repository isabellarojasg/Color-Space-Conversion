// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// RGB to YCC conversion

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "CSC_global.h"

// private data

// private prototypes
// =======
static void CSC_RGB_to_YCC_brute_force_float( Image pic);

// =======
static void CSC_RGB_to_YCC_brute_force_int(Image pic);

// =======
static uint8_t chrominance_downsample(
    uint8_t C_pixel_1, uint8_t C_pixel_2,
    uint8_t C_pixel_3, uint8_t C_pixel_4);

// private definitions
// =======
static void CSC_RGB_to_YCC_brute_force_float(Image pic) {

  yccImage Y;
  Y.width = pic.width; 
  Y.height =  pic.height;
  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * pic.height * pic.width);


  int i;
  for(i = 0; i< pic.height * pic.width; i+=2){
  Y.pixels[i].y = (uint8_t)(16.0 + 0.098*pic.pixels[i].b
                                   + 0.504*pic.pixels[i].g
                                   + 0.257*pic.pixels[i].r);
  Y.pixels[i+1].y = (uint8_t)(16.0 + 0.098*pic.pixels[i].b
                                   + 0.504*pic.pixels[i].g
                                   + 0.257*pic.pixels[i].r);

  Y.pixels[i].cb = (uint8_t)(128.0 + 0.439*pic.pixels[i].b
                                   - 0.291*pic.pixels[i].g
                                   - 0.148*pic.pixels[i].r);
  Y.pixels[i+1].cb = (uint8_t)(128.0 + 0.439*pic.pixels[i].b
                                   - 0.291*pic.pixels[i].g
                                   - 0.148*pic.pixels[i].r);
  Y.pixels[i].cr = (uint8_t)(128.0 - 0.071*pic.pixels[i].b
                                   - 0.368*pic.pixels[i].g
                                   + 0.439*pic.pixels[i].r);
  Y.pixels[i+1].cr = (uint8_t)(128.0 - 0.071*pic.pixels[i].b
                                   - 0.368*pic.pixels[i].g
                                   + 0.439*pic.pixels[i].r);
  // Cb[row>>1][col>>1] = chrominance_downsample( Cb_pixel_00,
  //                                              Cb_pixel_01,
  //                                              Cb_pixel_10,
  //                                              Cb_pixel_11);

  // Cr[row>>1][col>>1] = chrominance_downsample( Cr_pixel_00,
  //                                              Cr_pixel_01,
  //                                              Cr_pixel_10,
  //                                              Cr_pixel_11);
  }

    // Open the output file for writing
    FILE* output_file = fopen("outputYCC.txt", "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
    }

    // Write the YCC pixel components to the output file
    int j;
    for (j = 0; j < Y.width * Y.height; j++) {
            fprintf(output_file, "Pixel %d: Y=%d, Cb=%d, Cr=%d\n", j + 1,
              Y.pixels[j].y, Y.pixels[j].cb, Y.pixels[j].cr);
    }

    // Close the output file
    fclose(output_file);

    // Free allocated memory
    free(Y.pixels);

} // END of CSC_RGB_to_YCC_brute_force_float()

// =======
static void CSC_RGB_to_YCC_brute_force_int(Image pic) {
//
  int R_pixel_00, R_pixel_01, R_pixel_10, R_pixel_11;
  int G_pixel_00, G_pixel_01, G_pixel_10, G_pixel_11;
  int B_pixel_00, B_pixel_01, B_pixel_10, B_pixel_11;

  int  Y_pixel_00,  Y_pixel_01,  Y_pixel_10,  Y_pixel_11;
  int Cb_pixel_00, Cb_pixel_01, Cb_pixel_10, Cb_pixel_11;
  int Cr_pixel_00, Cr_pixel_01, Cr_pixel_10, Cr_pixel_11;

  yccImage Y;
  Y.width = pic.width; 
  Y.height =  pic.height;
  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * pic.height * pic.width);
   // Open the output file for writing
    FILE* output_file = fopen("outputYCC.txt", "wb");
    if (output_file == NULL) {
        perror("Error opening output file");
    }




  int i;
  for(i = 0; i< pic.height * pic.width; i+=4){
  Y_pixel_00 = (16 << (K)) + C13 * pic.pixels[i].b
                           + C12 * pic.pixels[i].g
                           + C11 * pic.pixels[i].r;
  Y_pixel_00 += (1 << (K-1)); // rounding
  Y_pixel_00 = Y_pixel_00 >> K;

  Y_pixel_01 = (16 << (K)) + C13 * pic.pixels[i+1].b
                           + C12 * pic.pixels[i+1].g
                           + C11 * pic.pixels[i+1].r;
  Y_pixel_01 += (1 << (K-1)); // rounding
  Y_pixel_01 = Y_pixel_01 >> K;

  Y_pixel_10 = (16 << (K)) + C13 * pic.pixels[i+2].b
                           + C12 * pic.pixels[i+2].g
                           + C11 * pic.pixels[i+2].r;
  Y_pixel_10 += (1 << (K-1)); // rounding
  Y_pixel_10 = Y_pixel_10 >> K;

  Y_pixel_11 = (16 << (K)) + C13 * pic.pixels[i+3].b
                           + C12 * pic.pixels[i+3].g
                           + C11 * pic.pixels[i+3].r;
  Y_pixel_11 += (1 << (K-1)); // rounding
  Y_pixel_11 = Y_pixel_11 >> K;


  Y.pixels[i].y = (uint8_t)Y_pixel_00;
  Y.pixels[i+1].y = (uint8_t)Y_pixel_01;
  Y.pixels[i+2].y = (uint8_t)Y_pixel_10;
  Y.pixels[i+3].y = (uint8_t)Y_pixel_11;

  Cb_pixel_00 = (128 << (K)) + C23 *  pic.pixels[i].b
                             - C22 *  pic.pixels[i].g
                             - C21 *  pic.pixels[i].r;
  Cb_pixel_00 += (1 << (K-1)); // rounding
  Cb_pixel_00 = Cb_pixel_00 >> K;

  Cb_pixel_01 = (128 << (K)) + C23 *  pic.pixels[i].b
                             - C22 *  pic.pixels[i].g
                             - C21 *  pic.pixels[i].r;
  Cb_pixel_01 += (1 << (K-1)); // rounding
  Cb_pixel_01 = Cb_pixel_01 >> K;

  Cb_pixel_10 = (128 << (K)) + C23 *  pic.pixels[i].b
                             - C22 *  pic.pixels[i].g
                             - C21 *  pic.pixels[i].r;
  Cb_pixel_10 += (1 << (K-1)); // rounding
  Cb_pixel_10 = Cb_pixel_10 >> K;

  Cb_pixel_11 = (128 << (K)) + C23 *  pic.pixels[i].b
                             - C22 *  pic.pixels[i].g
                             - C21 *  pic.pixels[i].r;
  Cb_pixel_11 += (1 << (K-1)); // rounding
  Cb_pixel_11 = Cb_pixel_11 >> K;

  Y.pixels[i].cb = (uint8_t)Cb_pixel_00;
  Y.pixels[i+1].cb = (uint8_t)Cb_pixel_01;
  Y.pixels[i+2].cb = (uint8_t)Cb_pixel_10;
  Y.pixels[i+3].cb = (uint8_t)Cb_pixel_11;

  Cr_pixel_00 = (128 << (K)) - C33 *  pic.pixels[i].b
                             - C32 *  pic.pixels[i].g
                             + C31 *  pic.pixels[i].r;
  Cr_pixel_00 += (1 << (K-1)); // rounding
  Cr_pixel_00 = Cr_pixel_00 >> K;

  Cr_pixel_01 = (128 << (K)) - C33 *  pic.pixels[i].b
                             - C32 *  pic.pixels[i].g
                             + C31 *  pic.pixels[i].r;
  Cr_pixel_01 += (1 << (K-1)); // rounding
  Cr_pixel_01 = Cr_pixel_01 >> K;

  Cr_pixel_10 = (128 << (K)) - C33 *  pic.pixels[i].b
                             - C32 *  pic.pixels[i].g
                             + C31 *  pic.pixels[i].r;
  Cr_pixel_10 += (1 << (K-1)); // rounding
  Cr_pixel_10 = Cr_pixel_10 >> K;

  Cr_pixel_11 = (128 << (K)) - C33 *  pic.pixels[i].b
                             - C32 *  pic.pixels[i].g
                             + C31 *  pic.pixels[i].r;
  Cr_pixel_11 += (1 << (K-1)); // rounding
  Cr_pixel_11 = Cr_pixel_11 >> K;


  Y.pixels[i].cr = (uint8_t)Cr_pixel_00;
  Y.pixels[i+1].cr = (uint8_t)Cr_pixel_01;
  Y.pixels[i+2].cr = (uint8_t)Cr_pixel_10;
  Y.pixels[i+3].cr = (uint8_t)Cr_pixel_11;
  // Cb[row>>1][col>>1] = chrominance_downsample( (uint8_t)Cb_pixel_00,
  //                                              (uint8_t)Cb_pixel_01,
  //                                              (uint8_t)Cb_pixel_10,
  //                                              (uint8_t)Cb_pixel_11);

  // Cr[row>>1][col>>1] = chrominance_downsample( (uint8_t)Cr_pixel_00,
  //                                              (uint8_t)Cr_pixel_01,
  //                                              (uint8_t)Cr_pixel_10,
  //                                              (uint8_t)Cr_pixel_11);
  fprintf(output_file, "Pixel %d: Y=%d, Cb=%d, Cr=%d\n", i,
              Y.pixels[i].y, Y.pixels[i].cb, Y.pixels[i].cr);
  }
    // Close the output file
    fclose(output_file);
    // printf("Pixel[%d]: y = %d, cb= %d, cr= %d\n", i, Y.pixels[0].y,Y.pixels[0].cb,Y.pixels[0].cr);
    // printf("Pixel[%d]: y = %d, cb= %d, cr= %d\n", i, Y.pixels[962].y,Y.pixels[962].cb,Y.pixels[962].cr);
    // printf("Pixel[%d]: y = %d, cb= %d, cr= %d\n", i, Y.pixels[2870].y,Y.pixels[2870].cb,Y.pixels[2870].cr);

    free(Y.pixels);

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
      switch (RGB_to_YCC_ROUTINE) {
        case 0:
          break;
        case 1:
          CSC_RGB_to_YCC_brute_force_float( pic);
          break;
        case 2:
          CSC_RGB_to_YCC_brute_force_int(pic);
          break;
        default:
          break;
      
//      printf( "Luma_00  = %02hhx\n", Y[row+0][col+0]);
//      printf( "Luma_01  = %02hhx\n", Y[row+0][col+1]);
//      printf( "Luma_10  = %02hhx\n", Y[row+1][col+0]);
//      printf( "Luma_11  = %02hhx\n\n", Y[row+1][col+1]);
    
  }

} // END of CSC_RGB_to_YCC()

