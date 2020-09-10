#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int rank, numprocs, M, N, count, remainder, myRowsSize;
	int* matrix = NULL;
	int* vector;
	int* result = NULL;
	int* sendcounts = NULL;
	int* senddispls = NULL;
	int* recvcounts = NULL;
	int* recvdispls = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* temp=new int[1];

	double start, end;
	if (0 == rank)start = MPI_Wtime();
	if (0 == rank)
	{
		cout << "Product of a vector by a matrix" << endl << endl;
		cout << "Enter the number of matrix rows/columns:" << endl;
		cin >> M;
		if (M < 1)	return EXIT_FAILURE;
	//	cout << "Enter the number of matrix columns:" << endl;
//		cin >> N;
		N = M;
		if (N < 1)	return EXIT_FAILURE;

		cout << endl;
		matrix = new int[M * N];
		// generate matrix
		//cout << "matrix:" << endl;
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				matrix[N * i + j] = rand() % 100;
				//cout << matrix[N * i + j] << '\t';
			}
			//cout << endl;
		}
		//cout << endl;

		delete[] temp;
		temp = new int[N];
		// generate vector
		//cout << "vector:" << endl;
		for (int i = 0; i < N; ++i)
		{
			temp[i] = rand() & 100;
			//cout << temp[i] << ' ';
		}
		//cout << endl << endl;

		sendcounts = new int[numprocs];
		senddispls = new int[numprocs];
		recvcounts = new int[numprocs];
		recvdispls = new int[numprocs];

		count = M / numprocs;
		remainder = M - count * numprocs;
		int prefixSum = 0;
		for (int i = 0; i < numprocs; ++i)
		{
			recvcounts[i] = (i < remainder) ? count + 1 : count;
			sendcounts[i] = recvcounts[i] * N;
			recvdispls[i] = prefixSum;
			senddispls[i] = prefixSum * N;
			prefixSum += recvcounts[i];
		}
	}

	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	vector = new int[N];
	for (int is = 0; is < N; ++is) {
		if (rank == 0)		vector[is] = temp[is];
	}
	delete[] temp;

	MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

	if (0 != rank)
	{
		count = M / numprocs;
		remainder = M - count * numprocs;
	}
	myRowsSize = rank < remainder ? count + 1 : count;
	int* matrixPart = new int[myRowsSize * N];
	MPI_Scatterv(matrix, sendcounts, senddispls, MPI_INT, matrixPart, myRowsSize * N, MPI_INT, 0, MPI_COMM_WORLD);
	if (0 == rank)
	{
		delete[] sendcounts;
		delete[] senddispls;
		delete[] matrix;
	}

	int* resultPart = new int[myRowsSize];

	for (int i = 0; i < myRowsSize; ++i)
	{
		resultPart[i] = 0;
		for (int j = 0; j < N; ++j)
		{
			resultPart[i] += matrixPart[i * N + j] * vector[j];
			//cout << "{";
		}
	}
	delete[] matrixPart;
	delete[] vector;

	if (0 == rank)
		result = new int[M];
	MPI_Gatherv(resultPart, myRowsSize, MPI_INT, result, recvcounts, recvdispls, MPI_INT, 0, MPI_COMM_WORLD);
	delete[] resultPart;
	if (0 == rank)
	{
		delete[] recvcounts;
		delete[] recvdispls;
	}

	if (0 == rank)end = MPI_Wtime();
	if (0 == rank)cout << end - start << endl;
	MPI_Finalize();

	if (0 == rank)
	{
		//cout << "result:" << endl;
		//for (int i = 0; i < M; ++i)
		//	cout << result[i] << ' ';
		//cout << endl;
		delete[] result;

	}
	return EXIT_SUCCESS;
}