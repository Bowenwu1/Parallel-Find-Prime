/**
 * Copyright @吴博文
 */
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;
bool whetherTrue(bool in) {
    return in;
}
int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Illegal Paramenter" << endl;
        return -1;
    }
    int n = atoi(argv[1]);
    n = pow(2, n) * 1000;
    MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // process start time
    double startTime = MPI_Wtime();

    int start, end;
    int seedEnd = (int)floor(sqrt(n));
    int partitionSizePerProcess = (n - seedEnd) / (world_size - 1);
    int extraSizeForLastRank = (n - seedEnd) % (world_size - 1);
    // decide start and end
    if (0 == world_rank) {
        start = 2;
        end = seedEnd;
    } else {
        start = seedEnd + partitionSizePerProcess * (world_rank - 1) + 1;
        end = start + partitionSizePerProcess - 1;
    }
    if (world_size - 1 == world_rank) {
        end += extraSizeForLastRank;
    }
    vector<bool> prime = vector<bool>(end - start + 1, true);

    // start finding prime
    if (0 == world_rank) {
        for (int num = 2; num <= seedEnd; ++num) {
            if (prime[num - start]) {
                // tell rank 1 to search for num
                MPI_Send(&num, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
#ifdef DEBUG
                printf("[SEND]Process %d send %d to process %d\n", world_rank, num, world_rank + 1);
#endif
                for (int loc = num << 1; loc <= end; loc += num) {
                    prime[loc - start] = false;
                }
            }
        }
        // send -1 as a signal to end this process
        int num = -1;
        MPI_Send(&num, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);        
    } else {
        while (1) {
            int num = 0;
            MPI_Recv(&num, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
            printf("[RECV]Process %d recv %d from process %d\n", world_rank, num, world_rank - 1);
#endif
            // pass to next
            if (world_size - 1 != world_rank) {
                MPI_Send(&num, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
#ifdef DEBUG
                printf("[SEND]Process %d send %d to process %d\n", world_rank, num, world_rank + 1);
#endif
            }
            // end signal
            if (num == -1) {
                break;
            }
            // decide starting point
            int startPoint;
            if (start % num == 0) {
                startPoint = start;
            } else {
                startPoint = start + (num - start % num);
            }
            for (int loc = startPoint; loc <= end; loc += num) {
                prime[loc - start] = false;
            }
        }
    }
    // now calculate each number of prime
    int count = count_if(prime.begin(), prime.end(), whetherTrue);
    int total;
    MPI_Reduce(&count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double endTime = MPI_Wtime();
    if (0 == world_rank) {
        cout << total << " primes less or equal " << n << endl;
        cout << "Spending " << endTime - startTime << " using " << world_size << " processes" << endl;
    }
    MPI_Finalize();
}