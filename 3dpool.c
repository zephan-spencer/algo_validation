#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/resource.h>

#define pool0InDim 10
#define pool0InChan 16
#define pool0KSize 2
#define pool0KernChan 16
#define pool0OutDim 5
#define pool0OutChan 16

#define InputIdx3D(i,j,k) (pool0InDim*pool0InDim*(k) + pool0InDim*(j) + i)
#define KIdx3D(i,j,k) (pool0KSize*pool0KSize*(k) + pool0KSize*(i) + j)
#define OutIdx3D(i,j,k) (pool0OutDim*pool0OutDim*(k) + pool0OutDim*(j/pool0KSize) + i/pool0KSize)

void main(int argc, char** argv) {
    int *convInput = (int*)malloc(pool0InDim * pool0InDim * pool0InChan * sizeof(int));
    int *kernel = (int*)malloc(pool0KSize * pool0KSize * pool0KernChan * sizeof(int));
    int *convOut = (int*)malloc(pool0OutDim * pool0OutDim * pool0OutChan * sizeof(int));

    int i, j, k, l, m, n;


    for (i = 0; i < pool0InDim; i++){
        for (j = 0; j < pool0InDim; j++){
            for(k = 0; k < pool0InChan; k++){
                convInput[i + j*pool0InDim + k*pool0InDim*pool0InDim] = i+j;
            }
        }
    }

    for (i = 0; i < pool0KSize; i++){
        for (j = 0; j < pool0KSize; j++){
            for(k = 0; k < pool0KernChan; k++){
                if(i == 0 && j == 0)
                kernel[i + j*pool0KSize + k*pool0KSize*pool0KSize] = 1;
            }
        }
    }

    for (i = 0; i < pool0OutDim; i++){
        for (j = 0; j < pool0OutDim; j++){
            for(k = 0; k < pool0OutChan; k++){
                convOut[i + j*pool0OutDim + k*pool0OutDim*pool0OutDim] = 0;
            }
        }
    }

    for (k = 0; k < 1; k++){
        for ( j = 0; j < pool0InDim; j+=2) {
            for ( i = 0; i < pool0InDim; i+=2) {
                int sum = 0;
                for (m = 0; m < pool0KSize; m++) {
                    for ( l = 0; l < pool0KSize; l++) {
                        sum += convInput[InputIdx3D(i+l, j+m, k)];
                        printf("Val:%d at %d 	", convInput[InputIdx3D(i+l, j+m, k)], (pool0InDim*(j+m) + i+l));
                    }
                }
                printf("Average:%d\n", sum/(pool0KSize*pool0KSize));
                convOut[OutIdx3D(i,j,k)] += sum/(pool0KSize*pool0KSize);
            }
        }
    }

    // Print out k
    // for(k = 0; k < pool0KernChan; k++){
    //     printf("Kernel:%d\n\n", k);
    //     for (i = 0; i < pool0KSize; i++){
    //         for (j = 0; j < pool0KSize; j++){
    //             printf("%d\t", kernel[i + j*pool0KSize + k*pool0KSize*pool0KSize]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }

    k = 0;
    for(k = 0; k < 1; k++){
        printf("Input:%d\n\n", k);
        for (i = 0; i < pool0InDim; i++){
            for (j = 0; j < pool0InDim; j++){
                printf("%d\t", convInput[i + j*pool0InDim + k*pool0InDim*pool0InDim]);
            }
            printf("\n");
        }
        printf("\n");
    }

    k = 0;
    for(k = 0; k < 1; k++){
        printf("Output:%d\n\n", k);
        for (i = 0; i < pool0OutDim; i++){
            for (j = 0; j < pool0OutDim; j++){
                printf("%d\t", convOut[i + j*pool0OutDim + k*pool0OutDim*pool0OutDim]);
            }
            printf("\n");
        }
        printf("\n");
    }
}