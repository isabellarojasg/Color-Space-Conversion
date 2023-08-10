// Copyright 2023 Mihai SIMA (mihai.sima@ieee.org).  All rights reserved.
// Color Space Conversion (CSC) in fixed-point arithmetic
// RGB to YCC conversion

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "CSC_global.h"

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
  int size = pic.width * pic.height;
  int width = pic.width;
  int height = pic.height;

  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * pic.height * pic.width);

  int i;
  //Can probably loop unroll up to 4 in parallel
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
  // Perform Downsampling
  yccDSPImage yDSP;
  yDSP.pixels = (yccDSPPixel *)malloc(sizeof(yccDSPPixel) * size >> 2);
  int k=0;
  int j;
  for(i = 0; i < Y.height >> 1; i++){
    for(j = 0; j < Y.width >> 1; j++){
      int index = i * 2 * width + j * 2;
      yDSP.pixels[k].y1 = Y.pixels[index].y;
      yDSP.pixels[k].y2 = Y.pixels[index+1].y;
      yDSP.pixels[k].y3 = Y.pixels[index+width].y;
      yDSP.pixels[k].y4 = Y.pixels[index+width+1].y;
      yDSP.pixels[k].cb = (Y.pixels[index].cb + Y.pixels[index+1].cb + Y.pixels[index+width].cb + Y.pixels[index+width+1].cb) >> 2;
      yDSP.pixels[k].cr = (Y.pixels[index].cr + Y.pixels[index+1].cr + Y.pixels[index+width].cr + Y.pixels[index+width+1].cr) >> 2;
      k++;
    }
  }
    // //Open the output file for writing
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
    free(yDSP.pixels);

    free(Y.pixels);
} // END of CSC_RGB_to_YCC()

