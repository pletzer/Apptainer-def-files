## `mpi_test.c`

* Run across multiple nodes (2 nodes, 2 processes per node by default)
* Print detailed information about each process including:

  - Hostname of each node
  - MPI processor name
  - Process ID
  - Rank distribution

<details>
<summary>Standard out</summary
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc005
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc005
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc006
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc006
  Local device: mlx5_0
--------------------------------------------------------------------------

=== MPI Test Program Overview ===
Total number of processes: 4

Process Distribution:
Rank 0 is running on node: wmc005
Rank 1 is running on node: wmc005
Rank 2 is running on node: wmc006
Rank 3 is running on node: wmc006

Detailed information for Rank 0:
  Hostname: wmc005
  MPI Processor Name: wmc005
  Process ID: 3410979

Detailed information for Rank 1:
  Hostname: wmc005
  MPI Processor Name: wmc005
  Process ID: 3410978

Detailed information for Rank 2:
  Hostname: wmc006
  MPI Processor Name: wmc006
  Process ID: 3205718

Detailed information for Rank 3:
  Hostname: wmc006
  MPI Processor Name: wmc006
  Process ID: 3205719

=== Test Complete ===
</details>



## `matrixmultiplympi_test.c`

* Performs parallel matrix multiplication of two 1000x1000 matrices
* Uses MPI collective operations (Bcast, Scatterv, Gatherv)
* Includes detailed timing and performance metrics
* Shows memory usage per process
* Includes a checksum verification

Standard output should show:

* Overall execution time
* Per-process statistics
* Node distribution
* Memory usage
* Result verification via checksum

<details>
<summary>Standard out</summary>
The following modules were not unloaded:
  (Use "module --force purge" to unload all):

  1) XALT/minimal   2) slurm   3) NeSI
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc005
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc005
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc006
  Local device: mlx5_0
--------------------------------------------------------------------------
--------------------------------------------------------------------------
WARNING: There was an error initializing an OpenFabrics device.

  Local host:   wmc006
  Local device: mlx5_0
--------------------------------------------------------------------------
Initializing matrices...

=== Parallel Matrix Multiplication Performance ===
Matrix size: 1000 x 1000
Number of processes: 4
Total execution time: 0.92 seconds
Result verification checksum: 2.499528e+08

Process Statistics:
Rank 0 running on wmc005:
  Rows processed: 250
  Block size used: 100
  Memory used: 4.58 MB

Process Statistics:
Rank 1 running on wmc005:
  Rows processed: 250
  Block size used: 100
  Memory used: 4.58 MB

Process Statistics:
Rank 2 running on wmc006:
  Rows processed: 250
  Block size used: 100
  Memory used: 4.58 MB

Process Statistics:
Rank 3 running on wmc006:
  Rows processed: 250
  Block size used: 100
  Memory used: 4.58 MB
</details>
