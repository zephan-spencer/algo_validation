#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/resource.h>


int main(int argc, char** argv){
        unsigned long long n = atoi(argv[1]);
        int i, j, k;

        clock_t t;

        int *a = (int*)malloc(n * n * sizeof(int));
        int *b = (int*)malloc(n * n * sizeof(int));
        int *c = (int*)malloc(n * n * sizeof(int));

        for (i = 0; i < n; i++){
          for (j = 0; j<n; j++){
            a[i*n + j] = 1;
            b[i*n + j] = 1;
            c[i*n + j] = 0;
          }
        }

        t = clock();
         /* perform MM */
         for (i = 0; i < n; i++){
           for (j = 0; j < n; j++){
             c[i*n + j] = 0;
             for (k = 0; k < n; k++){
               c[i*n + j] += a[i*n + j]*b[i*n + j];
             }
           }
         }

        t = clock() - t;

        for (i = 0; i < n; i++) {
          for (j = 0; j < n; j++) {
            printf("%d\t", *(a + i*n + j));
          }
          printf("\n");
        }

        printf("\n");

        for (i = 0; i < n; i++) {
          for (j = 0; j < n; j++) {
            printf("%d\t", *(c + i*n + j));
          }
          printf("\n");
        }

        printf("\n");

        double time_taken = ((double)t)/CLOCKS_PER_SEC * 1000.0; // in milliseconds

        printf("%.3f\n", time_taken);

        return 0;
}
