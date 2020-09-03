// For-Comparacion.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <chrono>

#define max 7500
using namespace std;
using namespace std::chrono;

double A[max][max], x[max], y[max];

void resety() {
    for (int i = 0; i < max; ++i) {
        y[i] = 0;
    }
    return;
}

void set() {
    for (int i = 0; i < max; ++i) {
        double rds = (rand() % 1000 )/100.0;
        x[i] = rds;
        for (int j = 0; j < max; ++j) {
            
            double rd = (rand() % 1000) / 100.0;
            A[i][j] = rd;
        }
    }
    resety();
    /*for (int i = 0; i < max; ++i) {
        for (int j = 0; j < max; ++j) {
            cout<<A[i][j]<<" ";
        }
        cout << endl;
    }

    for (int i = 0; i < max; ++i) {
        cout << x[i] << " " << y[i] << endl;
    }*/
}

void for1() {
    for (int i = 0; i < max; ++i) {
        for (int j = 0; j < max; ++j) {
            y[i] += A[i][j] * x[j];
        }
    }
    return;
}

void for2() {
    for (int j = 0; j < max; ++j) {
        for (int i = 0; i < max; ++i) {
            y[i] += A[i][j] * x[j];
        }
    }
    return;
}

int main()
{
    srand(time(NULL));
    set();
    auto start1 = high_resolution_clock::now();
    auto stop1 = high_resolution_clock::now();
    auto start2 = high_resolution_clock::now();
    auto stop2 = high_resolution_clock::now();
    for1();
    stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop1 - start1);
    cout << "Time taken by function: " << duration1.count() << " microseconds" << endl;

    resety();
    start2 = high_resolution_clock::now();
    for2();
    stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    cout << "Time taken by function: " << duration2.count() << " microseconds" << endl;

    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
