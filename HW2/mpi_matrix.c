# include <stdio.h>
# include <stdlib.h>
# include <mpi.h>
# include <time.h>

# define FROM_MASTER 1
# define FROM_SLAVE 2

int main (int argc, char *argv[]){
    int times_of_matrix;

    char *b_ = argv[1];
    times_of_matrix = atoi(b_);

    int NRA = 62;
    int NCA = 15;
    int NCB = 7;

    NRA *= times_of_matrix;
    NCA *= times_of_matrix;
    NCB *= times_of_matrix;

    // init. parameters
    int i, j, k;
    int process_num, process_id, slave_num;
    int dest, source;
    int rows, rows_aver, remainder, offset;
    double A[NRA][NCA], B[NCA][NCB], C[NRA][NCB];
    double start_time, end_time;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &process_num);

    slave_num = process_num -1;
    
    // if(numprocs < 2){
    //     printf("Error: Too few processes! \n");
    //     MPI_Abort(MPI_COMM_WORLD, 99);
    // }

    if(process_id == 0){
        // for init. the matrix
        // use the same init. way in omp_mm.c
        for(i = 0; i < NRA; i++){
            for(j = 0; j < NCA; j++){
                A[i][j] = i + j;
            }
        }
        for(i = 0; i< NCA; i++){
            for(j = 0; j < NCB; j++){
                B[i][j] = i * j;
            }
        }
        for(i = 0; i< NRA; i++){
            for(j = 0; j < NCB; j++){
                C[i][j] = 0;
            }
        }
        printf("NRA %d, NCA %d, NCB %d\n", NRA, NCA, NCB);
        printf("times of matrix:%d\n", times_of_matrix);

        rows_aver = NRA / slave_num;
        remainder = NRA % slave_num;
        offset = 0;

        start_time = MPI_Wtime();
        // sending row infos to each process
        for(dest = 1; dest <= slave_num; dest++){
            rows = (dest<= remainder) ? rows_aver+1 : rows_aver;
            // printf("sending %d rows to process %d\n", rows, dest); 
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], rows*NCA, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&B, NCA*NCB, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);

            offset += rows;
        }

        for(source = 1; source <= slave_num; source++){
            MPI_Recv(&offset, 1, MPI_INT, source, FROM_SLAVE, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, FROM_SLAVE, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows*NCB, MPI_DOUBLE, source, FROM_SLAVE, MPI_COMM_WORLD, &status);
        }

        end_time = MPI_Wtime();

        // printf("******************************************************\n");
        // printf("Result Matrix:\n");
        // for (i=0; i<NRA; i++){
        //     printf("%d", i);
        //     for (j=0; j<NCB; j++){
        //         printf("%6.2f   ", C[i][j]);
        //     }
        //     printf("\n"); 
        // }
        // printf("******************************************************\n");
        // printf ("Done.\n");


        printf( "%f seconds\n", end_time - start_time );
    }


        if(process_id > 0){

            MPI_Recv(&offset, 1, MPI_INT, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
            MPI_Recv(&A, rows*NCA, MPI_DOUBLE, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
            MPI_Recv(&B, NCA*NCB, MPI_DOUBLE, 0, FROM_MASTER, MPI_COMM_WORLD, &status);
        
            for(i=0; i<rows; i++){
                for(j=0; j<NCB; j++){
                    for(k=0; k<NCA; k++){
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }

            MPI_Send(&offset, 1, MPI_INT, 0, FROM_SLAVE, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, 0, FROM_SLAVE, MPI_COMM_WORLD);
            MPI_Send(&C, rows*NCB, MPI_DOUBLE, 0, FROM_SLAVE, MPI_COMM_WORLD);
            
        
        }
    
    // printf("process_id %d closed \n",process_id);
    MPI_Finalize();
    
    return 0;
}
