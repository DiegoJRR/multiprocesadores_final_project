#include <stdio.h>
#include <stdlib.h>

void read_matrix(char* filename, int n, int m, double (*matrix)[n]) {
    FILE *file = fopen(filename, "r");

    char buffer[256];
    double value;

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            fgets(buffer, 256, file);

            value = atof(buffer);
            matrix[i][j] = value;
        }
    }

    fclose(file);
}

void write_matrix(char* filename, int n, int m, double (*matrix)[n]) {
    FILE *file = fopen(filename, "w");

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            fprintf(file, "%f\n", matrix[i][j]);

        }
    }

    fclose(file);
}

void mult_mat(int n, int m, double (*A)[n], double (*B)[n], double (*C)[n]) {
    for (int rowNum = 0; rowNum < n; rowNum++) {
        for (int colNum = 0; colNum < m; colNum ++){
            double acum = 0;

            for (int i = 0; i < n; i++) {
                acum = acum + A[rowNum][i]*B[i][colNum];
            }

            C[rowNum][colNum] = acum;
        }
    }
}

int main() {
    int n, m;
    n = m = 50;

    double matrixA[n][m];
    double matrixB[n][m];
    double matrixC[n][m];
    read_matrix("matrixA2500.txt", n, m, matrixA);
    read_matrix("matrixB2500.txt", n, m, matrixB);
    // read_matrix("testSmallA.txt", n, m, matrixA);
    // read_matrix("testSmallB.txt", n, m, matrixB);

    mult_mat(n, m, matrixA, matrixB, matrixC);
    write_matrix("output.txt", n, m, matrixC);

    for (int i  = 0; i < n; i++) {
        for (int j  = 0; j < m; j++) {
            printf("%f\n", matrixC[i][j]);
        }
    }

    return 0;
}
