# Color-Space-Conversion

By: Isabella Rojas and Lydia Mekonnen Lulie

## Design Specifications 
In our design implementation, the selection of the image size of 64 x 48 pixels is influenced by the characteristics of the ARM instruction set and the cache size being used. The image size exceeds the capacity of the cache we are utilizing. The choice allows us to perform the color space conversion algorithm and assess the algorithm’s performance. Our goal is to maximize the algorithm’s efficiency within the confines of the ARM instruction set architecture.

## Blueprint of Design and Algorithm
![EVyVhod0RgX8T_4ImYS8UQWErnXDwykHvAcgswTdWW2YCInTe6fZOPrcyBEmzVUA_t94EkH80NPGm9CTXm8jubnDAo8BFOjRXgrRNbtDseXd7MrEHzfRUSevjNUg](https://github.com/isabellarojasg/Color-Space-Conversion/assets/68630621/b61310d0-54bf-4a3b-aa46-91da16f2dd3a)

## Work Plan

### Challenges
- We will need to conduct extensive research to improve precision when calculating color components in the algorithm.
- It will be challenging but feasible to find optimizations for the software implementation and refining the code. 
- Finding the bottleneck in the software functions and proposing a new instruction for performance improvement will be a challenge. 
- Estimating the performance improvement of the new instruction, estimating latency and speedup will be challenging to justify the choice of the new instruction over the pure-software solution.

### Optimizations
- Colorspace conversion is a data-dominant application therefore the approach used for accessing the cache can be optimized. 
- Vector (SIMD) operations can be optimized because the application has data-level parallelism
- For achieving an accurate image conversion various techniques for chrominance downsampling and upsampling will be considered.
- Improve precision by utilizing integer arithmetic and rounding techniques.
- Reduce the number of function classes as well as load and store instructions in both floating-point and integer arithmetic functions. This will optimize the execution process and improve efficiency and performance.
- Optimization flags to force an instruction set.
