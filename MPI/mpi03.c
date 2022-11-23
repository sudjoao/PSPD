#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>

#define MASTER_RANK 0
#define MESSAGE_SIZE 1
#define TAG 0

int receive_and_print_message(int rank){
    int received_msg;
    MPI_Status status;
    MPI_Recv(&received_msg, MESSAGE_SIZE, MPI_INT, rank == -1? MPI_ANY_SOURCE : rank, TAG, MPI_COMM_WORLD,  &status);
    printf("Processo %d  enviou o número: %d\n", status.MPI_SOURCE, received_msg);
    return received_msg;
}
int main(int argc, char * argv[]){
    int my_rank, nprocess;
    int received_msg, sended_value = 17;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess);

    if(!my_rank){
        for(int i=1; i<nprocess; i++){
            int specif_value = sended_value * i;
            MPI_Send(&specif_value, MESSAGE_SIZE, MPI_INT, i, TAG, MPI_COMM_WORLD);
            receive_and_print_message(-1);
        }
    } else {
        int received_value = receive_and_print_message(MASTER_RANK);
        int new_value = received_value + 2;
        MPI_Send(&new_value, MESSAGE_SIZE, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}


// Compilar: mpicc {nome_arquivo}
// Rodar: mpirun -n {numero_processos} {path_do_executavel}