# Color-Space-Conversion

By: Isabella Rojas and Lydia Mekonnen Lulie

## Commands to run the code:

### Unoptimized:
arm-linux-gcc -static  -march=armv6 -o CSC_main.exe CSC_main.c CSC_RGB_to_YCC_01.c CSC_YCC_to_RGB_01.c

#### Assembly:
arm-linux-gcc -static -S -march=armv6 CSC_main.c && arm-linux-gcc -static -S -march=armv6 CSC_RGB_to_YCC_01.c && arm-linux-gcc -static -S -march=armv6 CSC_YCC_to_RGB_01.c

### Optimized:
arm-linux-gcc -static -march=armv6 -o CSC_main.exe CSC_main.c

#### Assembly:
arm-linux-gcc -static -S -march=armv6 CSC_main.c:

### Optimized with -O3 flag:
arm-linux-gcc -static -march=armv6 -O3 -o CSC_main.exe CSC_main.c

#### Assembly:
arm-linux-gcc -static -S -O3 -march=armv6 CSC_main.c

### Running the executable and performance stats:
 perf stat -e task-clock,cycles,instructions,cache-references,cache-misses qemu-arm -cpu arm1176 CSC_main.exe 


