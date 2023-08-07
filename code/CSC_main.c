// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// main() function

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <string.h>
#include <math.h>
#include <sys/time.h>

#define GLOBAL
#include "CSC_global.h"

int main( void) {
  int row, col;
  FILE *f_ID_input_RGB;
  FILE *f_ID_echo_R;
  FILE *f_ID_echo_G;
  FILE *f_ID_echo_B;
  FILE *f_ID_output_Y;
  FILE *f_ID_output_Cb;
  FILE *f_ID_output_Cr;
  FILE *f_ID_output_RGB;
  struct timeval start_time, end_time;
  double latency;

  uint8_t R[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Red array pointer
 uint8_t G[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Green array pointer
 uint8_t B[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Blue array pointer
 uint8_t Y[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Luminance array pointer
 uint8_t Cb[IMAGE_ROW_SIZE >> 1][IMAGE_COL_SIZE >> 1]; // Chrominance (Cb) array pointer
 uint8_t Cr[IMAGE_ROW_SIZE >> 1][IMAGE_COL_SIZE >> 1]; // Chrominance (Cr) array pointer
 uint8_t Cb_temp[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Chrominance (Cb) temp array pointer
 uint8_t Cr_temp[IMAGE_ROW_SIZE][IMAGE_COL_SIZE]; // Chrominance (Cr) temp array pointer

  gettimeofday(&start_time, NULL); // Record the start time

  f_ID_input_RGB = fopen( "./image_86_64.data", "rb");
  if( f_ID_input_RGB == NULL) {
    printf( "Cannot open file input_RGB_64_48_03.\n");
    return( 1);
  }

  f_ID_echo_R = fopen( "./image_echo_R_64_48_03.data", "wb");
  if( f_ID_echo_R == NULL) {
    printf( "Cannot open file echo_R_64_48_03.\n");
    return( 1);
  }

  f_ID_echo_G = fopen( "./image_echo_G_64_48_03.data", "wb");
  if( f_ID_echo_G == NULL) {
    printf( "Cannot open file echo_G_64_48_03.\n");
    return( 1);
  }

  f_ID_echo_B = fopen( "./image_echo_B_64_48_03.data", "wb");
  if( f_ID_echo_B == NULL) {
    printf( "Cannot open file echo_B_64_48_03.\n");
    return( 1);
  }

  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col++) {
    R[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( R[row][col], f_ID_echo_R);

    G[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( G[row][col], f_ID_echo_G);

    B[row][col] = (uint8_t)( fgetc( f_ID_input_RGB));
    fputc( B[row][col], f_ID_echo_B);
  }
  fclose( f_ID_echo_B);
  fclose( f_ID_echo_G);
  fclose( f_ID_echo_R);
  fclose( f_ID_input_RGB);

  CSC_RGB_to_YCC(R,G,B,Y,CB,CR);

  f_ID_output_Y = fopen( "./image_output_Y_64_48_03.data", "wb");
  if( f_ID_output_Y == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Y_64_48_03.data");
    return( 1);
  }
  
  f_ID_output_Cb = fopen( "./image_output_Cb_64_48_03.data", "wb");
  if( f_ID_output_Cb == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Cb_64_48_03.data");
    return( 1);
  }
  
  f_ID_output_Cr = fopen( "./image_output_Cr_64_48_03.data", "wb");
  if( f_ID_output_Cr == NULL) {
    fprintf( stderr, "Could not open %s\n", 
             "./image_output_Cr_64_48_03.data");
    return( 1);
  }
  
  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col++) {
    //fprintf( f_ID_output_Y, "%02hhx", Y[row][col]);
    fputc( Y[row][col], f_ID_output_Y);
  }

  for( row=0; row < (IMAGE_ROW_SIZE >> 1); row++)
  for( col=0; col < (IMAGE_COL_SIZE >> 1); col++) {
    //fprintf( f_ID_output_Cb, "%02hhx", Cb[row][col]);
    fputc( Cb[row][col], f_ID_output_Cb);
    //fprintf( f_ID_output_Cr, "%02hhx", Cr[row][col]);
    fputc( Cr[row][col], f_ID_output_Cr);
  }

  fclose( f_ID_output_Cr);
  fclose( f_ID_output_Cb);
  fclose( f_ID_output_Y);

  CSC_YCC_to_RGB();

  f_ID_output_RGB = fopen( "./image_output_RGB_64_48_03.data", "wb");
  if( f_ID_output_RGB == NULL) {
    printf( "Cannot open file RGB_64_48_03.\n");
    return( 1);
  }

  for( row=0; row < IMAGE_ROW_SIZE; row++)
  for( col=0; col < IMAGE_COL_SIZE; col++) {
    fputc( R[row][col], f_ID_output_RGB);
    fputc( G[row][col], f_ID_output_RGB);
    fputc( B[row][col], f_ID_output_RGB);
  }
  fclose( f_ID_output_RGB);

  gettimeofday(&end_time, NULL); // Record the end time

  // Calculate the latency in seconds
    latency = (end_time.tv_sec - start_time.tv_sec) +
              (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Latency: %f seconds\n", latency);
} // END of main()

