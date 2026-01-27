#include "pch.h"
#include <iostream>
#include <omp.h>

#define N 10000        // Tamaño de los arreglos
#define CHUNK 100      // Cantidad de iteraciones asignadas a cada hilo
#define MOSTRAR 20     // Elementos a imprimir por arreglo

// Imprime los primeros MOSTRAR valores de un arreglo
void imprimeArreglo(const float* d);

int main()
{
    std::cout << "Sumando Arreglos en Paralelo!\n";

    // Número de hilos solicitados a OpenMP
    int NUM_HILOS = 50;
    omp_set_num_threads(NUM_HILOS);

    // Consulta de información del sistema
    int num_procs = omp_get_num_procs();      // CPUs disponibles
    int max_threads = omp_get_max_threads();  // Máximo permitido por OpenMP

    std::cout << "CPUs disponibles        : " << num_procs << std::endl;
    std::cout << "Hilos configurados      : " << NUM_HILOS << std::endl;
    std::cout << "Max hilos OpenMP        : " << max_threads << std::endl;

    float a[N], b[N], c[N];

    // Se llenan los arreglos a y b con valores conocidos
    for (int i = 0; i < N; i++)
    {
        a[i] = i * 10.0f;        // Valor proporcional al índice
        b[i] = (i + 3) * 3.7f;   // Valor distinto para verificar la suma
    }

    // Región paralela usada solo para comprobar cuántos hilos se crean
#pragma omp parallel
    {
        // single evita que todos los hilos impriman el mensaje
#pragma omp single
        {
            std::cout << "Threads realmente creados: "
                << omp_get_num_threads() << std::endl;
        }
    }

    // Cada hilo suma una porción del arreglo usando bloques fijos (static)
#pragma omp parallel for schedule(static, CHUNK)
    for (int i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
    }

    std::cout << "\nImprimiendo los primeros " << MOSTRAR
        << " valores del arreglo a:\n";
    imprimeArreglo(a);

    std::cout << "Imprimiendo los primeros " << MOSTRAR
        << " valores del arreglo b:\n";
    imprimeArreglo(b);

    std::cout << "Imprimiendo los primeros " << MOSTRAR
        << " valores del arreglo c:\n";
    imprimeArreglo(c);

    return 0;
}

// Función auxiliar para mostrar resultados parciales
void imprimeArreglo(const float* d)
{
    for (int x = 0; x < MOSTRAR; x++)
        std::cout << d[x] << " - ";
    std::cout << std::endl;
}
