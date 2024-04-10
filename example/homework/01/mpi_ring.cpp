#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    int rank, procs;
    int ring_num;
    int N;

    int stop = -1;
    if (argc < 2) {
        cout << "Please add an input for the times around the ring" << endl;
        return 1;
    }
    else {
        N = atoi(argv[1]);
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ring_num = 0;
        MPI_Send(&ring_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    while(1)
    {
        if (rank == 0) {
            MPI_Recv(&ring_num, 1, MPI_INT, procs-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ring_num/(procs-1) == N) {
                MPI_Send(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                break;
            }
            ring_num++;
            MPI_Send(&ring_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        else {
            MPI_Recv(&ring_num, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ring_num == -1) {
                MPI_Send(&stop, 1, MPI_INT, (rank+1)%procs, 0, MPI_COMM_WORLD);
                break;
            }
            ring_num++;
            MPI_Send(&ring_num, 1, MPI_INT, (rank+1)%procs, 0, MPI_COMM_WORLD);
        }
    }
    if (rank == 0) {
        cout << "The ring has looped " << N << " times" << endl;
        cout << "end number: " << ring_num << endl;
    }
    MPI_Finalize();

    return 0;
}
