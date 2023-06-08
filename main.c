#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int read_matrix(char* filename, int n, int m, double* matrix) {
    FILE *file = fopen(filename, "r");

    char buffer[256];
    double value;

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            char* found = fgets(buffer, 256, file);

            value = atof(buffer);
            if (! value) {
                printf("Not enough lines in file '%s'", filename);
                return 1;
            }
            matrix[i * m + j] = value;
        }
    }
    
    // We expect one empty line at the end of the file
    fgets(buffer, 256, file);
    if (fgets(buffer, 256, file)) {
        printf("More lines than expected in file '%s'", filename);
        return 1;
    }

    fclose(file);
    return 0;
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

double mult_mat(int n, int m, int p, double* A, double* B, double* C) {
    //  For each row in A matrix, we do a dot product with the corresponding B column 
    //  And save this result in C
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int rowNum = 0; rowNum < n; rowNum++) {
        for (int colNum = 0; colNum < p; colNum ++){
            double acum = 0;

            for (int i = 0; i < m; i++) {
                acum += A[rowNum*m + i]*B[i*p + colNum];
            }

            C[rowNum*p + colNum] = acum;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    return elapsed;
}

double openmp_mult(int n, int m, int p, double* A, double* B, double* C) {
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel for num_threads(8)
    for (int rowNum = 0; rowNum < n; rowNum++) {
        // printf("Executing on thread %d/%d\n", omp_get_thread_num(), omp_get_max_threads());
        for (int colNum = 0; colNum < p; colNum ++){
            double acum = 0;

            for (int i = 0; i < m; i++) {
                acum += A[rowNum*m + i]*B[i*p + colNum];
            }

            C[rowNum*p + colNum] = acum;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    return elapsed;
}

void get_matrix_shape(int *n, int *m, char *matrixLetter) {
    printf("Enter the shape for matrix %s (n x m)\n", matrixLetter);
    printf("First, the number of rows (n):\n");
    scanf("%d", n);  
    printf("Now the number of columns (m):\n");
    scanf("%d", m);  
    printf("The shape of matrix %s is %d x %d\n", matrixLetter, *n, *m);
}

void print_results(double *serial_times, double *openmp_times, double *cuda_times) {
    printf("Corrida | Serial     | OpenMP (8 threads)   | CUDA         |\n");

    float serial_avg = 0;
    float openmp_avg = 0;
    float cuda_avg = 0;

    for (int iter = 0; iter < 5; iter++) {
        serial_avg += serial_times[iter]/5.0;
        openmp_avg += openmp_times[iter]/5.0;
        cuda_avg += cuda_times[iter]/5.0;

        printf("%d       |", iter + 1);
        printf(" %f    |   %f    |   %f    | \n", serial_times[iter], openmp_times[iter], cuda_times[iter]);
    }

    printf("Promedio | %f | %f | %f |\n", serial_avg, openmp_avg, cuda_avg);
    printf("%% vs Serial |   -   | %f | %f |\n", openmp_avg/serial_avg, cuda_avg/serial_avg);
}

int compare_matrix(double* A, double* B, int n, int m) {
    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            if(A[i*m + j] != B[i*m + j]) {
                return 1;
            }
        }
    }

    return 0;
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

    // Validate matrix shapes for multiplication 
    if(m != o) {
        printf("The matrix A with size (%dx%d) cannot be multiplied with matrix B of size (%dx%d)", n, m, p, o);
        return 1;
    }
    
    double* matrixA = malloc((n*m) * sizeof(double));
    double* matrixB = malloc((o*p) * sizeof(double));
    double* matrixC = malloc((n*p) * sizeof(double));
    double* temp = malloc((n*p) * sizeof(double));

    // Execution time arrays
    double serial_times[5];
    double openmp_times[5];
    double cuda_times[5];

    // TODO: Test this properly
    if(matrixA == NULL || matrixB == NULL || matrixC == NULL) {
        printf("The matrices are too large, not enough memory to allocate them\n");
        return 1;
    }

    if(read_matrix(argv[1], n, m, matrixA)) {
        return 1;
    };

    if(read_matrix(argv[2], o, p, matrixB)) {
        return 1;
    }

    for(int iteration = 0; iteration < 5; iteration ++) {
        double time_serial = mult_mat(n, m, p, matrixA, matrixB, matrixC);
        double omp_time = openmp_mult(n, m, p, matrixA, matrixB, temp);
        if(compare_matrix(matrixC, temp, n, p)) {
            printf("Error in OpenMP code, matrix does not match serial version\n");
        } else {
            printf("OpenMP version gives same matrix as serial!\n");
        }

        // double cuda_time = openmp_mult(n, m, p, matrixA, matrixB, matrixC);
        
        // TODO: Execute the other methods properly
        serial_times[iteration] = time_serial;
        openmp_times[iteration] = omp_time;
        cuda_times[iteration] = time_serial;
    }

    write_matrix("matrizC.txt", n, p, matrixC);

    print_results(serial_times, openmp_times, cuda_times);
    return 0;
}
