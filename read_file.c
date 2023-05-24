#include <stdio.h>
#include <stdlib.h>

void read_matrix(char* filename, int n, int m, double (*matrix)[n]) {
    FILE *file = fopen(filename, "r");

    char buffer[256];
    int i = 0;
    int j = 0;
    double value;
    while(fgets(buffer, 256, file) && i < n && j < m) {
        value = atof(buffer);
        matrix[i][j] = value;

        i++;

        if(i >= n) {
            j++;
            i = 0;
        }
    }

    fclose(file);
}

int main() {
    double matrixA[50][50];
    double matrixB[50][50];
    read_matrix("matrixA2500.txt", 50, 50, matrixA);
    read_matrix("matrixB2500.txt", 50, 50, matrixB);

    for (int i  = 0; i < 50; i++) {
        for (int j  = 0; j < 50; j++) {
            printf("%f\n", matrixA[i][j]);
        }
    }

    return 0;
}
