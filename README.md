# NTU_HPBDAIS
108-2 : High-Performance Big Data and Artificial Intelligence Systems (高效能巨量資料與人工智慧系統)

## HW1
1. Here is an OpenMP example of Matrix Multiply 
https://computing.llnl.gov/tutorials/openMP/samples/C/omp_mm.c
2. Compile and run it on a server in the CSIE Workstation Lab… Make sure it runs correctly
3. Comment out the printf statements. Run it with 1, 2, 4, 8,… threads and report the execution time
4. Double the values of NRA, NCA, and NCB to observe the execution time.
5. Repeat Step 4 until a problem happens to the system. Report your observations.
### usage
```bash=
gcc hw.1.c -o <exec_file_name> -fopenmp
./<exec_file_name> <num_of_threads> <times_of_matrix>
```

## HW2
1. Write an MPI code to perform the same matrix multiply task as Exercise 1. You can get the code from anywhere, but make sure that you understand how it works.
2. Install the MPI environment, compile and run the MPI code on a server in the CSIE Workstation Lab.with 1, 2, 4, 8, … processes… Report the execution time. Make sure it runs correctly.
3. Run it on multiple servers and report the execution time.
4. Double the values of NRA, NCA, and NCB to observe the execution time.
### usage
```bash=
mpicc -g -Wall -o <exec_file_name> mpi_matrix.c
mpiexec -n <num_of_threads> ./<exec_file_name> <times_of_matrix>
```
