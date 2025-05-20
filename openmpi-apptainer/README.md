# OpenMPI-Apptainer

There are few additional `UCX` variables in the definition file which may not get used during runtime. They will trigger a warning which can be silenced with `UCX_WARN_UNUSED_ENV_VARS=n`

* For an example, `srun --mpi=pmi2 apptainer exec --env UCX_WARN_UNUSED_ENV_VARS=n openmpi-4.1.5.aimg ./matrixmultiply_test` 


## Slurm template for the tests

```bash
#!/bin/bash -e

#SBATCH --job-name=mpi_test
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2
#SBATCH --mem-per-cpu=2G
#SBATCH --time=00:10:00
#SBATCH --output=slog/%j.out

# Load any necessary modules here if required
module purge
module load  Apptainer
#Compile
#apptainer exec openmpi.sif mpicc -o tests/mpi_test mpi_test.c
#apptainer exec openmpi-4.1.5.aimg mpicc -o matrixmultiply_test tests/matrixmultiplympi_test.c -lm

# Run the MPI test program
srun --mpi=pmi2 apptainer exec --env UCX_WARN_UNUSED_ENV_VARS=n openmpi-4.1.5.aimg ./matrixmultiply_test
```

##  *OpenMPI/4.1.5-GCC-12.3.0* vs *Apptainer* runtime comparison for  `matrixmultiplympi_test.c` ( `MATRIX_SIZE 7000`)

![image](./images/openmpi_runtime.png)

<details>
<summary>R script</summary>

```r
library(ggplot2)
library(stringr)

# Create the data frame
data <- data.frame(
  Implementation = c("Env.Module (OpenMPI/4.1.5-GCC-12.3.0)", "Apptainer"),
  Runtime = c(157, 179)
)

# Create the plot with default ggplot2 settings
ggplot(data, aes(x = Implementation, y = Runtime, fill = Implementation)) +
  geom_col(width = 0.2) +
  geom_text(aes(label = Runtime), vjust = -0.5, size = 4) +
  labs(
    title = expression(atop(
      "Runtime Comparison: Env.Module vs Apptainer",
      paste(italic("matrixmultiplympi_test.c"), " (MATRIX_SIZE 7000)")
    )),
    x = "OpenMPI Implementation",
    y = "Runtime (seconds)"
  ) +
  theme_grey(base_size = 15) +
  theme(
    legend.position = "none",
    plot.title = element_text(hjust = 0.5, lineheight = 1.2)
  )
```
</details>