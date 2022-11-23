#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>

#define MASTER_RANK 0
#define MESSAGE_SIZE 100

int main(int argc, char * argv[]){
    int my_rank;
    char msg[MESSAGE_SIZE];
    int tag = 0;
    int nprocess;
    MPI_Status status;
    // Inicia o MPI
    MPI_Init(&argc, &argv);

    // Define um rank para o MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess);

    // printf("Ola mundo! Meu pid= %d\nMeu rank: %d\n", getpid(), my_rank);

    if(my_rank != MASTER_RANK){
         int numCharacters = sprintf(msg, "Noooooooo, by: %d", my_rank);
        MPI_Send(msg, numCharacters+1, MPI_CHAR, MASTER_RANK, tag, MPI_COMM_WORLD);
    } else {
        for(int i=1; i<nprocess; i++){
            printf("I'm your father\n");
            MPI_Recv(msg, MESSAGE_SIZE, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            printf("Processo %d enviou a mensagem: \n'%s'\n", status.MPI_SOURCE, msg);
        }
    }

    // Finaliza o MPI
    MPI_Finalize();

    return 0;
}


// Compilar: mpicc {nome_arquivo}
// Rodar: mpirun -n {numero_processos} {path_do_executavel}