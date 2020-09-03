// Multiplicacion_matrices.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#define A_x 1000
#define A_y 1000

#define B_x 1000
#define B_y 1000

#define max 5
#define tam 400

using namespace std;
using namespace std::chrono;

int A[A_x][A_y];
int B[B_x][B_y];
int res[A_x][B_y];
int ser[A_x][B_y];


void set() {

	for (int i = 0; i < A_x; ++i) {
		for (int j = 0; j < A_y; ++j) {
			A[i][j] = rand() % max;
		}
	}
	for (int i = 0; i < B_x; ++i) {
		for (int j = 0; j < B_y; ++j) {
			B[i][j] = rand() % max;
		}
	}
	return;
}

void mult_clasica() {
	for (int i = 0; i < A_x; ++i) {
		for (int j = 0; j < B_y; ++j) {
			for (int k = 0; k < A_y; ++k) {
				res[i][j] = res[i][j] + (A[i][k]*B[k][j]);
			}
		}
	}
	cout << "Resultado Mult_Clasica :" << endl;
	for (int i = 0; i < A_x; ++i) {
		for (int j = 0; j < B_y; ++j) {
			//cout << res[i][j] << " ";
		}
	//	cout << endl;
	}
}

void mult_bloques() {
	int temp;
	int N = A_x;
	int s = tam;
	for (int jj = 0; jj < N; jj += s) {
		for (int kk = 0; kk < N; kk += s) {
			for (int i = 0; i < N; i++) {
				for (int j = jj; j < ((jj + s) > N ? N : (jj + s)); j++) {
					temp = 0;
					for (int k = kk; k < ((kk + s) > N ? N : (kk + s)); k++) {
						temp += A[i][k] * B[k][j];
					}
					ser[i][j] += temp;
				}
			}
		}
	}
	cout << "Resultado Mult_Bloques :" << endl;
	for (int i = 0; i < A_x; ++i) {
		for (int j = 0; j < B_y; ++j) {
			//cout << ser[i][j] << " ";
		}
		//cout << endl;
	}
}

int main()
{
	srand(time(NULL));
	set();


	auto start1 = high_resolution_clock::now();
	auto stop1 = high_resolution_clock::now();
	auto start2 = high_resolution_clock::now();
	auto stop2 = high_resolution_clock::now();
	mult_clasica();
	stop1 = high_resolution_clock::now();
	auto duration1 = duration_cast<microseconds>(stop1 - start1);
	cout << "Time taken by function: " << duration1.count() << " microseconds" << endl;

	start2 = high_resolution_clock::now();
	mult_bloques();
	stop2 = high_resolution_clock::now();
	auto duration2 = duration_cast<microseconds>(stop2 - start2);
	cout << "Time taken by function: " << duration2.count() << " microseconds" << endl;

	//mult_bloques();

	cout << "A : " << endl;
	for (int i = 0; i < A_x; ++i) {
		for (int j = 0; j < A_y; ++j) {
	//		cout << A[i][j] << " ";
		}
	//	cout << endl;
	}
	cout << endl;
	cout << "B : " << endl;
	for (int i = 0; i < B_x; ++i) {
		for (int j = 0; j < B_y; ++j) {
//			cout << B[i][j] << " ";
		}
//		cout << endl;
	}
	cout << endl;
	
	return 0;
}
