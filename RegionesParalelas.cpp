#include "pch.h"
#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>

#define N 100000000       // Tamaño de los arreglos
#define CHUNK 100    // Cantidad de iteraciones asignadas a cada hilo
#define MOSTRAR 20    // Elementos a imprimir por arreglo

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

    // Arreglos en heap
    std::vector<float> a(N), b(N), c(N);

    // Inicialización de arreglos
    for (int i = 0; i < N; i++)
    {
        a[i] = i * 10.0f;
        b[i] = (i + 3) * 3.7f;
    }

    // Región paralela solo para verificar número de hilos
#pragma omp parallel
    {
#pragma omp single
        {
            std::cout << "Threads realmente creados: "
                << omp_get_num_threads() << std::endl;
        }
    }

    // Inicio del timer
    auto start = std::chrono::high_resolution_clock::now();

    // Suma paralela
#pragma omp parallel for schedule(static, CHUNK)
    for (int i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
    }

    // Fin del timer
    auto end = std::chrono::high_resolution_clock::now();

    // Tiempo transcurrido en milisegundos
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "\nTiempo de ejecucion (suma paralela): "
        << elapsed.count() << " ms\n";

    std::cout << "\nImprimiendo los primeros " << MOSTRAR
        << " valores del arreglo a:\n";
    imprimeArreglo(a.data());

    std::cout << "Imprimiendo los primeros " << MOSTRAR
        << " valores del arreglo b:\n";
    imprimeArreglo(b.data());

    std::cout << "Imprimiendo los primeros " << MOSTRAR
        << " valores del arreglo c:\n";
    imprimeArreglo(c.data());

    return 0;
}

// Función auxiliar para mostrar resultados parciales
void imprimeArreglo(const float* d)
{
    for (int x = 0; x < MOSTRAR; x++)
        std::cout << d[x] << " - ";
    std::cout << std::endl;
}
