#include <stdio.h>
#include <stdlib.h>

extern void calculate_acceleration(float* matrix, int rows, int* output);

int main() {
    int rows;
    printf("Enter number of cars (rows): ");
    scanf_s("%d", &rows);

    // Allocate memory
    float* matrix = (float*)malloc(rows * 3 * sizeof(float));
    int* output = (int*)malloc(rows * sizeof(int));

    if (!matrix || !output) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Input the matrix values
    printf("Enter matrix values (Initial Velocity, Final Velocity, Time for each car):\n");
    for (int i = 0; i < rows; i++) {
        printf("Car %d (Vi Vf T): ", i + 1);
        scanf_s("%f %f %f", &matrix[i * 3], &matrix[i * 3 + 1], &matrix[i * 3 + 2]);

        // Debug print input values
        printf("Read values: Vi=%.2f, Vf=%.2f, T=%.2f\n",
            matrix[i * 3], matrix[i * 3 + 1], matrix[i * 3 + 2]);
    }

    // Call assembly function
    calculate_acceleration(matrix, rows, output);

    // Print the results
    printf("\nFinal Results - Integer acceleration values (m/s^2):\n");
    for (int i = 0; i < rows; i++) {
        printf("Car %d: %d\n", i + 1, output[i]);
    }

    // Free allocated memory
    free(matrix);
    free(output);
    return 0;
}