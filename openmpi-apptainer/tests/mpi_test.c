#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int world_size, world_rank;
    char hostname[256];
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get number of processes and rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name(processor_name, &name_len);
    gethostname(hostname, sizeof(hostname));

    // Prepare a buffer for gathering all hostnames
    char *all_hostnames = NULL;
    if (world_rank == 0) {
        all_hostnames = (char*)malloc(256 * world_size);
    }

    // Gather hostnames to rank 0
    MPI_Gather(hostname, 256, MPI_CHAR, all_hostnames, 256, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Process 0 prints the overview
    if (world_rank == 0) {
        printf("\n=== MPI Test Program Overview ===\n");
        printf("Total number of processes: %d\n", world_size);
        printf("\nProcess Distribution:\n");
        for (int i = 0; i < world_size; i++) {
            printf("Rank %d is running on node: %s\n", i, all_hostnames + i * 256);
        }
        free(all_hostnames);
    }

    // Each process prints its detailed information
    // Sleep for a short time between ranks to avoid output mixing
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < world_size; i++) {
        if (i == world_rank) {
            printf("\nDetailed information for Rank %d:\n", world_rank);
            printf("  Hostname: %s\n", hostname);
            printf("  MPI Processor Name: %s\n", processor_name);
            printf("  Process ID: %d\n", getpid());
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (world_rank == 0) {
        printf("\n=== Test Complete ===\n");
    }

    MPI_Finalize();
    return 0;
}
