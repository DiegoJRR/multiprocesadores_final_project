#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
            fprintf(file, "%.10f\n", matrix[i*m + j]);
        }
    }

    fclose(file);
}

int mult_mat(int n, int m, int p, double* A, double* B, double* C) {
    //  For each row in A matrix, we do a dot product with the corresponding B column 
    //  And save this result in C
    clock_t start, end;
    start = clock();

    for (int rowNum = 0; rowNum < n; rowNum++) {
        for (int colNum = 0; colNum < p; colNum ++){
            double acum = 0;

            for (int i = 0; i < m; i++) {
                acum += A[rowNum*m + i]*B[i*p + colNum];
            }

            C[rowNum*p + colNum] = acum;
        }
    }

    end = clock();
    return end-start;
}

void get_matrix_shape(int *n, int *m, char *matrixLetter) {
    printf("Enter the shape for matrix %s (n x m)\n", matrixLetter);
    printf("First, the number of rows (n):\n");
    scanf("%d", n);  
    printf("Now the number of columns (m):\n");
    scanf("%d", m);  
    printf("The shape of matrix %s is %d x %d\n", matrixLetter, *n, *m);
}

void print_results(int *serial_times, int *openmp_times, int *cuda_times) {
    printf("Corrida | Serial     | OpenMP       | CUDA         |\n");

    float serial_avg = 0;
    float openmp_avg = 0;
    float cuda_avg = 0;

    for (int iter = 0; iter < 5; iter++) {
        serial_avg += serial_times[iter]/5.0;
        openmp_avg += openmp_times[iter]/5.0;
        cuda_avg += cuda_times[iter]/5.0;

        printf("%d       |", iter + 1);
        printf(" %d    |   %d    |   %d    | \n", serial_times[iter], openmp_times[iter], cuda_times[iter]);
    }

    printf("Promedio | %f | %f | %f |\n", serial_avg, openmp_avg, cuda_avg);
    printf("%% vs Serial |   -   | %f | %f |\n", openmp_avg/serial_avg, cuda_avg/serial_avg);
}

int main(int arge, char *argv[]) {
    if(arge < 2) {
        printf("No arguments given");
        return 1;
    } 

    int n, m;
    int o, p;
    n = p = 1024;
    m = o = 1024;
    
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

    // Execution time arrays
    int serial_times[5];
    int openmp_times[5];
    int cuda_times[5];

    // TODO: Test this properly
    if(matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("The matrices are too large, not enough memory to allocate them\n");
        return 1;
    }

    read_matrix(argv[1], n, m, matrixA);
    read_matrix(argv[2], o, p, matrixB);

    for(int iteration = 0; iteration < 5; iteration ++) {
        int time_to_mult = mult_mat(n, m, p, matrixA, matrixB, matrixC);
        // printf("Time to mult serial: %d ms\n", time_to_mult);
        
        // TODO: Execute the other methods properly
        serial_times[iteration] = time_to_mult;
        openmp_times[iteration] = time_to_mult;
        cuda_times[iteration] = time_to_mult;
    }

    write_matrix("matrizC.txt", n, p, matrixC);

    print_results(serial_times, openmp_times, cuda_times);
    return 0;
}
