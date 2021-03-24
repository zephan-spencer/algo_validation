#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/resource.h>

#define TYPE int

#define conv1InDim 14
#define conv1InChan 6
#define conv1KSize 5
#define conv1KernChan 16
#define conv1OutDim 10
#define conv1OutChan 16

// HWC Memory Accesses
#define InputIdx3D(h,w,c) ((h * conv1InDim*conv1InChan + w * conv1InChan + c))
#define KIdx4D(h,w,c,n) ((n * conv1KSize*conv1KSize*conv1InChan + h *conv1KSize*conv1InChan + w * conv1InChan + c))
#define OutIdx3D(h,w,c) ((h * conv1OutDim*conv1OutChan + w * conv1OutChan + c))

void main(int argc, char** argv) {
    int *convInput = (int*)malloc(conv1InDim * conv1InDim * conv1InChan * sizeof(int));
    int *kernel = (int*)malloc(conv1KSize * conv1KSize * conv1InChan * conv1KernChan * sizeof(int));
    int *convOut = (int*)malloc(conv1OutDim * conv1OutDim * conv1OutChan * sizeof(int));

    int i, j, k, l, m, n;

    for (i = 0; i < conv1InDim; i++){
        for (j = 0; j < conv1InDim; j++){
            for(k = 0; k < conv1InChan; k++){
                convInput[InputIdx3D(i,j,k)] = i+j;
            }
        }
    }

    for (n = 0; n < conv1OutChan; n++) {
        for (i = 0; i < conv1KSize; i++){
            for (j = 0; j < conv1KSize; j++){
                for(k = 0; k < conv1InChan; k++){
                    if(i == 2 && j == 2) {
                     kernel[KIdx4D(i,j,k,n)] = 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < conv1OutDim; i++){
        for (j = 0; j < conv1OutDim; j++){
            for(k = 0; k < conv1OutChan; k++){
                convOut[OutIdx3D(i,j,k)] = 0;
            }
        }
    }

    // HWC Implementation for Convolution
    int h,w,c,cc,x,y;
    // Input X
    for (h = 0; h < conv1OutDim; h++) {
        // Input Y
        for (w = 0; w < conv1InDim; w++) {
            // Check that the window is valid
            if(!(w+conv1KSize>conv1InDim || h+conv1KSize>conv1InDim)) {
                // Output Channels
                for(cc = 0; cc < conv1OutChan; cc++) {
                    // Kernel X
                    for (x = 0; x < conv1KSize; x++) {
                        // Kernel Y
                        for (y = 0; y < conv1KSize; y++) {
                            // Input Channels
                            int sum = 0;
                            for(c = 0; c < conv1InChan; c++) {
                                sum += convInput[InputIdx3D(h+x, w+y, c)]
                                * kernel[KIdx4D(x,y,c,cc)];
                            }
                            convOut[OutIdx3D(h,w,cc)] += sum;
                        }
                    }
                }
            }
        }
    }

    // // Print out k
    // for(n = 0; n < 1; n++){
    //     for(k = 0; k < 1; k++){
    //         printf("Kernel:%d\n\n", k);
    //         for (i = 0; i < conv1KSize; i++){
    //             for (j = 0; j < conv1KSize; j++){
    //                 printf("%d\t", kernel[KIdx4D(i,j,k,n)]);
    //             }
    //             printf("\n");
    //         }
    //         printf("\n");
    //     }
    // }

    // k = 0;
    // for(k = 0; k < 1; k++){
    //     printf("Input:%d\n\n", k);
    //     for (i = 0; i < conv1InDim; i++){
    //         for (j = 0; j < conv1InDim; j++){
    //             printf("%d\t", convInput[InputIdx3D(i,j,k)]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }

    k = 0;
    for(k = 0; k < 16; k++){
        printf("Output:%d\n\n", k);
        for (i = 0; i < conv1OutDim; i++){
            for (j = 0; j < conv1OutDim; j++){
                printf("%d\t", convOut[OutIdx3D(i,j,k)]);
            }
            printf("\n");
        }
        printf("\n");
    }
}