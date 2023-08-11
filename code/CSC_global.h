#include <stdint.h>

#define K 8 // bitwidth
#define UNITY (1 << K)
#define IMAGE_ROW_SIZE 86
#define IMAGE_COL_SIZE 64

// RGB_to_YCC_ROUTINE
//     1 for CSC_RGB_to_YCC_brute_force_float()
//     2 for CSC_RGB_to_YCC_brute_force_int()
#define RGB_to_YCC_ROUTINE 1

// YCC_to_RGB_ROUTINE
//     1 for CSC_YCC_to_RGB_brute_force_float()
//     2 for CSC_YCC_to_RGB_brute_force_int()
#define YCC_to_RGB_ROUTINE 1

// CHROMINANCE_DOWNSAMPLING_MODE = 
//     0 for returning zero (no chrominance)
//     1 for discarding three pixels and keeping one
//     2 for averaging four pixels
#define CHROMINANCE_DOWNSAMPLING_MODE 1

// CHROMINANCE_UPSAMPLING_MODE = 
//     0 for returning zero (no chrominance)
//     1 for replicating one pixel into three
//     2 for interpolation with two pixels
#define CHROMINANCE_UPSAMPLING_MODE 1

// RGB-to-YCC coefficients in 8-bit representation
#define C11  66
#define C12 129
#define C13  25
#define C21  38
#define C22  74
#define C23 112
#define C31 112
#define C32  94
#define C33  18

// YCC-to-RGB coefficients in 8-bit representation
#define D1  74
#define D2 102
#define D3  52
#define D4  25
#define D5 129




