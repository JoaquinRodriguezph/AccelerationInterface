#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>    // For QueryPerformanceCounter

extern void calculate_acceleration(float* matrix, int rows, int* output);

// Function to verify results
void verify_results(float* matrix, int* output, int rows) {
    const float conversion = 0.277777778f; // 1000/3600 for km/h to m/s
    int errors = 0;

    for (int i = 0; i < rows; i++) {
        float vi = matrix[i * 3] * conversion;     // Convert to m/s
        float vf = matrix[i * 3 + 1] * conversion; // Convert to m/s
        float t = matrix[i * 3 + 2];
        float expected = (vf - vi) / t;
        int rounded_expected = (int)roundf(expected);

        if (output[i] != rounded_expected) {
            printf("Verification Error at row %d: Expected %d, Got %d\n",
                i, rounded_expected, output[i]);
            errors++;
        }
    }

    if (errors == 0) {
        printf("All results verified correctly!\n");
    }
    else {
        printf("Total errors found: %d\n", errors);
    }
}

int read_from_file(const char* filename, float** matrix, int* rows) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    fscanf_s(file, "%d\n", rows);
    *matrix = (float*)malloc(*rows * 3 * sizeof(float));
    if (!*matrix) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < *rows; i++) {
        float vi, vf, t;
        if (fscanf_s(file, "%f,%f,%f\n", &vi, &vf, &t) != 3) {
            printf("Error reading line %d from file\n", i + 1);
            free(*matrix);
            fclose(file);
            return 0;
        }

        (*matrix)[i * 3] = vi;
        (*matrix)[i * 3 + 1] = vf;
        (*matrix)[i * 3 + 2] = t;
    }

    fclose(file);
    return 1;
}

void run_performance_test(float* matrix, int rows) {
    printf("\n=== Starting Performance Test for Input File Data ===\n");
    const int iterations = 30;

    int* output = (int*)malloc(rows * sizeof(int));
    if (!output) {
        printf("Memory allocation failed for output!\n");
        return;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    double total_time = 0;

    // Run multiple iterations
    for (int iter = 0; iter < iterations; iter++) {
        QueryPerformanceCounter(&start);
        calculate_acceleration(matrix, rows, output);
        QueryPerformanceCounter(&end);

        // Convert to microseconds
        double iteration_time = ((double)(end.QuadPart - start.QuadPart) * 1000000.0) / frequency.QuadPart;
        total_time += iteration_time;

        // Print results only for first iteration
        if (iter == 0) {
            printf("\nResults from first iteration:\n");
            for (int i = 0; i < rows; i++) {
                printf("Car %d: %d m/s^2\n", i + 1, output[i]);
            }
            verify_results(matrix, output, rows);
        }
    }

    // Calculate and print statistics
    double avg_time = total_time / iterations;
    printf("\nPerformance Statistics:\n");
    printf("Number of iterations: %d\n", iterations);
    printf("Average execution time: %.2f microseconds\n", avg_time);
    printf("Total time for all iterations: %.2f microseconds\n", total_time);
    printf("Data size (rows): %d\n", rows);
    printf("Average time per row: %.4f microseconds\n", avg_time / rows);

    free(output);
    printf("\n=== Performance Test Complete ===\n");
}

int main() {
    int choice;
    printf("Choose input method:\n");
    printf("1. Manual input\n");
    printf("2. Read from file\n");
    printf("Enter choice (1-2): ");
    scanf_s("%d", &choice);

    switch (choice) {
    case 1: {
        // Manual input
        int rows;
        printf("Enter number of cars (rows): ");
        scanf_s("%d", &rows);

        float* matrix = (float*)malloc(rows * 3 * sizeof(float));
        int* output = (int*)malloc(rows * sizeof(int));

        if (!matrix || !output) {
            printf("Memory allocation failed!\n");
            return -1;
        }

        printf("Enter matrix values (Initial Velocity, Final Velocity, Time for each car):\n");
        for (int i = 0; i < rows; i++) {
            printf("Car %d (Vi Vf T): ", i + 1);
            scanf_s("%f %f %f", &matrix[i * 3], &matrix[i * 3 + 1], &matrix[i * 3 + 2]);
        }

        calculate_acceleration(matrix, rows, output);

        printf("\nResults:\n");
        for (int i = 0; i < rows; i++) {
            printf("Car %d: %d m/s^2\n", i + 1, output[i]);
        }

        verify_results(matrix, output, rows);
        free(matrix);
        free(output);
        break;
    }
    case 2: {
        // File input
        char filename[20];
        int file_choice;

        printf("Select input file:\n");
        printf("1. input_10.txt\n");
        printf("2. input_100.txt\n");
        printf("3. input_1000.txt\n");
        printf("4. input_10000.txt\n");
        printf("Enter choice (1-4): ");
        scanf_s("%d", &file_choice);

        // Map choice to the corresponding file
        switch (file_choice) {
        case 1:
            strcpy_s(filename, sizeof(filename), "input_10.txt");
            break;
        case 2:
            strcpy_s(filename, sizeof(filename), "input_100.txt");
            break;
        case 3:
            strcpy_s(filename, sizeof(filename), "input_1000.txt");
            break;
        case 4:
            strcpy_s(filename, sizeof(filename), "input_10000.txt");
            break;
        default:
            printf("Invalid file choice!\n");
            return -1;
        }

        float* matrix;
        int rows;

        if (!read_from_file(filename, &matrix, &rows)) {
            printf("Failed to read input file\n");
            return -1;
        }

        printf("Successfully read %d rows from %s\n", rows, filename);
        run_performance_test(matrix, rows);
        free(matrix);
        break;
    }
    default:
        printf("Invalid choice!\n");
        return -1;
    }

    return 0;
}

