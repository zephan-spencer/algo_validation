#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/resource.h>

#define conv1InDim 14
#define conv1InChan 6
#define conv1KSize 5
#define conv1KernChan 16
#define conv1OutDim 10
#define conv1OutChan 16

#define InputIdx3D(i,j,k) (conv1InDim*conv1InDim*(k) + conv1InDim*(j) + i)
#define KIdx3D(i,j,k) (conv1KSize*conv1KSize*(k) + conv1KSize*(i) + j)
#define OutIdx3D(i,j,k) (conv1OutDim*conv1OutDim*(k) + conv1OutDim*(j-conv1KSize/2) + i-conv1KSize/2)

void main(int argc, char** argv) {
    int *convInput = (int*)malloc(conv1InDim * conv1InDim * conv1InChan * sizeof(int));
    int *kernel = (int*)malloc(conv1KSize * conv1KSize * conv1KernChan * sizeof(int));
    int *convOut = (int*)malloc(conv1OutDim * conv1OutDim * conv1OutChan * sizeof(int));

    int i, j, k, l, m, n;


    for (i = 0; i < conv1InDim; i++){
        for (j = 0; j < conv1InDim; j++){
            for(k = 0; k < conv1InChan; k++){
                convInput[i + j*conv1InDim + k*conv1InDim*conv1InDim] = i+j;
            }
        }
    }

    for (i = 0; i < conv1KSize; i++){
        for (j = 0; j < conv1KSize; j++){
            for(k = 0; k < conv1KernChan; k++){
                if(i == 0 && j == 0)
                kernel[i + j*conv1KSize + k*conv1KSize*conv1KSize] = 1;
            }
        }
    }

    for (i = 0; i < conv1OutDim; i++){
        for (j = 0; j < conv1OutDim; j++){
            for(k = 0; k < conv1OutChan; k++){
                convOut[i + j*conv1OutDim + k*conv1OutDim*conv1OutDim] = 0;
            }
        }
    }

    for (n = 0; n < conv1OutChan; n++){
        for (k = 0; k < conv1InChan; k++){
            for ( j = 0; j < conv1InDim; j++) {
                for ( i = 0; i < conv1InDim; i++) {
                    if(!(i-conv1KSize/2<0
                        || j-conv1KSize/2<0
                        || i+conv1KSize/2>=conv1InDim
                        || j+conv1KSize/2 >= conv1InDim)){
                        int sum = 0;
                        for ( m = -conv1KSize/2; m < conv1KSize/2; m++) {
                            for ( l = -conv1KSize/2; l < conv1KSize/2; l++) {
                                sum += convInput[InputIdx3D(i+l, j+m, k)]
                                * kernel[KIdx3D(l+conv1KSize/2, m+conv1KSize/2, n)];
                            }
                        }
                        convOut[OutIdx3D(i,j,n)] += sum;
                    }
                }
            }
        }
    }

    // Print out k
    // for(k = 0; k < conv1KernChan; k++){
    //     printf("Kernel:%d\n\n", k);
    //     for (i = 0; i < conv1KSize; i++){
    //         for (j = 0; j < conv1KSize; j++){
    //             printf("%d\t", kernel[i + j*conv1KSize + k*conv1KSize*conv1KSize]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }

    k = 0;
    for(k = 0; k < conv1InChan; k++){
        printf("Input:%d\n\n", k);
        for (i = 0; i < conv1InDim; i++){
            for (j = 0; j < conv1InDim; j++){
                printf("%d\t", convInput[i + j*conv1InDim + k*conv1InDim*conv1InDim]);
            }
            printf("\n");
        }
        printf("\n");
    }

    k = 0;
    for(k = 0; k < conv1OutChan; k++){
        printf("Output:%d\n\n", k);
        for (i = 0; i < conv1OutDim; i++){
            for (j = 0; j < conv1OutDim; j++){
                printf("%d\t", convOut[i + j*conv1OutDim + k*conv1OutDim*conv1OutDim]);
            }
            printf("\n");
        }
        printf("\n");
    }
}