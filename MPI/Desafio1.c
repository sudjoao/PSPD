#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>

#define MASTER_RANK 0
#define MESSAGE_SIZE 1
#define ARRAY_SIZE 16
#define TAG 0

void send_array_to_worker(int worker_num, int *array){
    MPI_Send(array, ARRAY_SIZE, MPI_INT, worker_num, TAG, MPI_COMM_WORLD);
}

int receive_array_from_worker(int worker_num, int new_array[], int source){
    MPI_Status status;
    MPI_Recv(new_array, ARRAY_SIZE, MPI_INT, source == -1? MPI_ANY_SOURCE : source, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
    return status.MPI_SOURCE;
}


void print_array(int* array, int size){
    printf("[");
    for(int i=0; i<size; i++){
        printf("%d", array[i]);
        if(i==size - 1){
            printf("]\n");
        } else{
            printf(", ");
        }
    }
}



int main(int argc, char * argv[]){
    int my_rank, nprocess;
    int a[ARRAY_SIZE], b[ARRAY_SIZE], c[ARRAY_SIZE];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess);


    if(!my_rank){
        int **answers;

        for(int i=0; i<ARRAY_SIZE; i++){
            a[i] = i;
            b[i] = (ARRAY_SIZE - i)*2;
        }
        printf("Array 1 enviado:\n");
        print_array(a, ARRAY_SIZE);
        printf("Array 2 enviado:\n");
        print_array(b, ARRAY_SIZE);
        for(int i=1; i<nprocess; i++){
            send_array_to_worker(i, a);
            send_array_to_worker(i, b);
        }

        for(int i=1; i< nprocess; i++){
            int sender = receive_array_from_worker(1, c, i);
            int qtt = ARRAY_SIZE / (nprocess - 1);
            if(ARRAY_SIZE % (nprocess - 1) && sender == nprocess - 1){
                qtt++;
            }
            printf("Array Calculado pelo worker %d:\n", sender);
            print_array(c, qtt);
        }

    } else {
        receive_array_from_worker(MASTER_RANK, a, 0);
        receive_array_from_worker(MASTER_RANK, b, 0);
        int qtt = ARRAY_SIZE / (nprocess - 1);
        int init_loop = (my_rank - 1) * qtt;
        if(ARRAY_SIZE % (nprocess - 1) && my_rank == nprocess - 1){
            qtt++;
        }
        int j=0;
        for(int i=init_loop; i<init_loop+qtt; i++){
            c[j++]= a[i]+b[i];
        }
        send_array_to_worker(0, c);
    }

    MPI_Finalize();
    return 0;
}


// Compilar: mpicc {nome_arquivo}
// Rodar: mpirun -n {numero_processos} {path_do_executavel}