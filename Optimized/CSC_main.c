#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <string.h>
#include <math.h>


#pragma pack(push, 1)

typedef struct {
	uint8_t y1;
    uint8_t y2;
    uint8_t y3;
    uint8_t y4;
	uint8_t cb;
	uint8_t cr;  

} yccDSPPixel;

typedef struct {
	int width;
	int height;
  yccDSPPixel *pixels;
} yccDSPImage;

typedef struct {
	uint8_t y;
	uint8_t cb;
	uint8_t cr;  

} yccPixel;

typedef struct {
	int width;
	int height;
  yccPixel *pixels;
} yccImage;

//represents a pixel in an image storing in RGB format
typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;  

} Pixel;

typedef struct {
	int width;
	int height;
  Pixel *pixels;
} Image;

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

Image CSC_YCC_to_RGB(yccDSPImage yccDSP, int height, int width){

  //Upsample yccDSP
  yccImage Y;
  Y.width = width; 
  Y.height =  height;
  Y.pixels = (yccPixel*) malloc (sizeof(yccPixel) * height * width);

  int k=0;
  int j,i;
  for(i = 0; i < height >> 1; i++){
    for(j = 0; j < width >> 1; j++){
      int index = i * 2 * width + j * 2;

      Y.pixels[index].y = yccDSP.pixels[k].y1;
      Y.pixels[index+1].y = yccDSP.pixels[k].y2;
      Y.pixels[index+width].y = yccDSP.pixels[k].y3;
      Y.pixels[index+width+1].y = yccDSP.pixels[k].y4;

      Y.pixels[index].cb = yccDSP.pixels[k].cb;
      Y.pixels[index+1].cb = yccDSP.pixels[k].cb;
      Y.pixels[index+width].cb = yccDSP.pixels[k].cb;
      Y.pixels[index+width+1].cb = yccDSP.pixels[k].cb;
     
      Y.pixels[index].cr = yccDSP.pixels[k].cr;
      Y.pixels[index+1].cr = yccDSP.pixels[k].cr;
      Y.pixels[index+width].cr = yccDSP.pixels[k].cr;
      Y.pixels[index+width+1].cr = yccDSP.pixels[k].cr;

      k++;
    }
  }

  //YCC to RGB Conversion
  Image yccRGB;
  yccRGB.height = height;
  yccRGB.width = width;
  int size = yccRGB.height * yccRGB.width;
  yccRGB.pixels = (Pixel*) malloc (sizeof(Pixel) * size);

  for(i = 0; i< size; i++){

    int y1 = 74*(Y.pixels[i].y -16);
    int r1 = (y1 + 102*(Y.pixels[i].cr - 128)) >> 6;
    int g1 = ((y1 - 52*(Y.pixels[i].cr - 128) - 25*(Y.pixels[i].cb - 128))) >> 6;
    int b1 = (y1 + 129*(Y.pixels[i].cb - 128)) >> 6;

    yccRGB.pixels[i].r = r1 > 255 ? 255 : (r1 < 0 ? 0 : r1);
    yccRGB.pixels[i].g = g1 > 255 ? 255 : (g1 < 0 ? 0 : g1);
    yccRGB.pixels[i].b = b1 > 255 ? 255 : (b1 < 0 ? 0 : b1);
  }

  free(Y.pixels);
  return yccRGB;


}

yccDSPImage CSC_RGB_to_YCC(Image pic) {
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
    
    free(Y.pixels);
    return yDSP;
} // END of CSC_RGB_to_YCC()


int main( void) {

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

    printf("Width: %d\n", info_header.width);
    printf("Height: %d\n", info_header.height);
    
    // Get the image dimensions
    int width = info_header.width;
    int height = abs(info_header.height); // Height can be negative for top-down BMP

   // Calculate padding (if any) at the end of each row in BMP

    Image pic;
    pic.height = height;
    pic.width = width;
    int size = pic.height * pic.width;
    pic.pixels = (Pixel*) malloc (sizeof(Pixel) * size);

    // int padding = (4 - (width * sizeof(Pixel)) % 4) % 4;
    // printf("padding: %d\n", padding);

        // Get the data offset
    uint32_t dataOffset = file_header.offset;

    printf("Data Offset: %u\n", dataOffset);
    // Move to the beginning of pixel data
    fseek(file, dataOffset, SEEK_SET);
    fread(pic.pixels, sizeof(Pixel), size, file);

   // Image image = readImage(file, height, width);
    fclose(file);

    yccDSPImage yccDSP = CSC_RGB_to_YCC(pic);


    // //Open the output file for writing
    // FILE* output_file = fopen("outputDownsampled.txt", "wb");
    // if (output_file == NULL) {
    //     perror("Error opening output file");
    // }

    // Write the YCC pixel components to the output file
    // int j;
    // for (j = 0; j < 720; j++) {
    //         fprintf(output_file, "Pixel %d: y1=%d, y2=%d,y3=%d,y4=%d,cb=%d,cr=%d,\n", j + 1,
    //          yccDSP.pixels[j].y1,  yccDSP.pixels[j].y2,  yccDSP.pixels[j].y3,  yccDSP.pixels[j].y4, yccDSP.pixels[j].cb, yccDSP.pixels[j].cr);
    // }
    // fclose(output_file);


    //Write RGB output to bitmap file
    FILE* outputRGBfile = fopen("./outputRGB.bmp", "wb");
    if (outputRGBfile == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
  
    Image outputImage = CSC_YCC_to_RGB(yccDSP, pic.height, pic.width);
  
  
    // //Open the output file for writing
    // FILE* output_file = fopen("outputRGB.txt", "wb");
    // if (output_file == NULL) {
    //     perror("Error opening output file");
    // }

    // int j;
    // for (j = 0; j < outputImage.height * outputImage.width; j++) {
    //         fprintf(output_file, "Pixel %d: b=%d, g=%d,r=%d\n", j + 1,
    //          outputImage.pixels[j].b,  outputImage.pixels[j].g,  outputImage.pixels[j].r);
    // }
    // fclose(output_file);

    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, outputRGBfile);
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, outputRGBfile);
    fseek (outputRGBfile, file_header.offset, SEEK_SET);
    int k;
    for(k = 0; k < size; k ++){
      fwrite(&outputImage.pixels[k], sizeof(Pixel), 1, outputRGBfile);
    }

    fclose(outputRGBfile);
    free(yccDSP.pixels);
    free(outputImage.pixels);
    free(pic.pixels);

} // END of main()

