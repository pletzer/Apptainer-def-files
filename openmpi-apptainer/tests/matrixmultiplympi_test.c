#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <string.h>  // Added for memcpy

#define MATRIX_SIZE 7000  // Reduced from 2000 to 1000
#define BLOCK_SIZE 100    // Process data in smaller blocks
#define MASTER 0

void initialize_matrix(double* matrix, int rows, int cols, int seed) {
    srand(seed);
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

void multiply_matrix_block(double* A, double* B, double* C, 
                         int rows_A, int cols_A, int cols_B) {
    for (int i = 0; i < rows_A; i++) {
        for (int j = 0; j < cols_B; j++) {
            double sum = 0.0;
            for (int k = 0; k < cols_A; k++) {
                sum += A[i * cols_A + k] * B[k * cols_B + j];
            }
            C[i * cols_B + j] = sum;
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, provided;
    double start_time, end_time;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

    // Calculate block size for each process
    int rows_per_proc = MATRIX_SIZE / size;
    int remainder = MATRIX_SIZE % size;
    int my_rows = (rank < remainder) ? rows_per_proc + 1 : rows_per_proc;
    int my_offset = rank * rows_per_proc + ((rank < remainder) ? rank : remainder);

    // Allocate matrices
    double *A = NULL, *B = NULL, *C = NULL, *local_A = NULL, *local_C = NULL;
    double *B_block = NULL;

    local_A = (double*)malloc(my_rows * MATRIX_SIZE * sizeof(double));
    local_C = (double*)malloc(my_rows * MATRIX_SIZE * sizeof(double));
    B_block = (double*)malloc(BLOCK_SIZE * MATRIX_SIZE * sizeof(double));

    // Initialize local_C to zero
    memset(local_C, 0, my_rows * MATRIX_SIZE * sizeof(double));

    if (rank == MASTER) {
        A = (double*)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
        B = (double*)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
        C = (double*)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double));
        
        printf("Initializing matrices...\n");
        initialize_matrix(A, MATRIX_SIZE, MATRIX_SIZE, 12345);
        initialize_matrix(B, MATRIX_SIZE, MATRIX_SIZE, 67890);
    }

    start_time = MPI_Wtime();

    // Scatter matrix A
    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        sendcounts[i] = (i < remainder ? rows_per_proc + 1 : rows_per_proc) * MATRIX_SIZE;
        displs[i] = (i * rows_per_proc + (i < remainder ? i : remainder)) * MATRIX_SIZE;
    }

    MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
                 local_A, my_rows * MATRIX_SIZE, MPI_DOUBLE,
                 MASTER, MPI_COMM_WORLD);

    // Broadcast matrix B in blocks
    for (int i = 0; i < MATRIX_SIZE; i += BLOCK_SIZE) {
        int current_block_size = (i + BLOCK_SIZE > MATRIX_SIZE) ? 
                               (MATRIX_SIZE - i) : BLOCK_SIZE;
        
        if (rank == MASTER) {
            memcpy(B_block, &B[i * MATRIX_SIZE], 
                   current_block_size * MATRIX_SIZE * sizeof(double));
        }
        
        MPI_Bcast(B_block, current_block_size * MATRIX_SIZE, MPI_DOUBLE, 
                  MASTER, MPI_COMM_WORLD);
        
        // Use received block for computation
        for (int j = 0; j < my_rows; j++) {
            for (int k = 0; k < current_block_size; k++) {
                for (int l = 0; l < MATRIX_SIZE; l++) {
                    local_C[j * MATRIX_SIZE + l] += 
                        local_A[j * MATRIX_SIZE + (i + k)] * 
                        B_block[k * MATRIX_SIZE + l];
                }
            }
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Gather results
    MPI_Gatherv(local_C, my_rows * MATRIX_SIZE, MPI_DOUBLE,
                C, sendcounts, displs, MPI_DOUBLE,
                MASTER, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (rank == MASTER) {
        printf("\n=== Parallel Matrix Multiplication Performance ===\n");
        printf("Matrix size: %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
        printf("Number of processes: %d\n", size);
        printf("Total execution time: %.2f seconds\n", end_time - start_time);
        
        // Calculate checksum
        double checksum = 0.0;
        for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
            checksum += C[i];
        }
        printf("Result verification checksum: %.6e\n", checksum);
    }

    // Print per-process statistics
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < size; i++) {
        if (i == rank) {
            printf("\nProcess Statistics:\n");
            printf("Rank %d running on %s:\n", rank, processor_name);
            printf("  Rows processed: %d\n", my_rows);
            printf("  Block size used: %d\n", BLOCK_SIZE);
            printf("  Memory used: %.2f MB\n", 
                   (my_rows * MATRIX_SIZE * 2 + BLOCK_SIZE * MATRIX_SIZE) * 
                   sizeof(double) / (1024.0 * 1024.0));
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // Cleanup
    free(local_A);
    free(local_C);
    free(B_block);
    free(sendcounts);
    free(displs);
    if (rank == MASTER) {
        free(A);
        free(B);
        free(C);
    }

    MPI_Finalize();
    return 0;
}
