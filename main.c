#include <stdio.h>
#include <stdlib.h>

// TODO: If matrix does not fit in memory, exit and show error
// TODO: Error if number of lines does not match NxM

void read_matrix(char* filename, int n, int m, double* matrix) {
    FILE *file = fopen(filename, "r");

    char buffer[256];
    double value;

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            fgets(buffer, 256, file);

            value = atof(buffer);
            matrix[i * m + j] = value;
        }
    }

    fclose(file);
}

void write_matrix(char* filename, int n, int m, double* matrix) {
    FILE *file = fopen(filename, "w");

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            fprintf(file, "%lf\n", matrix[i*m + j]);

        }
    }

    fclose(file);
}

void mult_mat(int n, int m, int p, double* A, double* B, double* C) {
    //  For each row in A matrix, we do a dot product with the corresponding B column 
    //  And save this result in C

    for (int rowNum = 0; rowNum < n; rowNum++) {
        for (int colNum = 0; colNum < p; colNum ++){
            double acum = 0;

            for (int i = 0; i < m; i++) {
                acum += A[rowNum*m + i]*B[i*p + colNum];
            }

            C[rowNum*p + colNum] = acum;
        }
    }
}

void get_matrix_shape(int *n, int *m, char *matrixLetter) {
    printf("Enter the shape for matrix %s (n x m)\n", matrixLetter);
    printf("First, the number of rows (n):\n");
    scanf("%d", n);  
    printf("Now the number of columns (m):\n");
    scanf("%d", m);  
    printf("The shape of matrix %s is %d x %d\n", matrixLetter, *n, *m);
}

int main(int arge, char *argv[]) {
    if(arge < 2) {
        printf("No arguments given");
        return 1;
    } 

    int n, m;
    int o, p;
    n = p = 1000;
    m = o = 10;
    
    // Ask the user for the shape of the matrix
    // get_matrix_shape(&n, &m, "A");
    // get_matrix_shape(&p, &o, "B");

    // TODO: NxM should correspond to the number of lines in the file 
    // Validate matrix shapes for multiplication 
    if(m != o) {
        printf("The matrix A with size (%dx%d) cannot be multiplied with matrix B of size (%dx%d)", n, m, p, o);
        return 1;
    }
    
    double* matrixA = malloc((n*m) * sizeof(double));
    double* matrixB = malloc((o*p) * sizeof(double));
    double* matrixC = malloc((n*p) * sizeof(double));

    // TODO: Test this properly
    if(matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("The matrices are too large, not enough memory to allocate them\n");
        return 1;
    }

    read_matrix(argv[1], n, m, matrixA);
    read_matrix(argv[2], o, p, matrixB);

    mult_mat(n, m, p, matrixA, matrixB, matrixC);
    write_matrix("output.txt", n, p, matrixC);

    // for (int i  = 0; i < n; i++) {
    //     for (int j  = 0; j < m; j++) {
    //         printf("%f\n", matrixC[i*m + j]);
    //     }
    // }

    printf("Finished program correctly\n");
    return 0;
}
