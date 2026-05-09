#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "reparto_camisetas_BT.h" // Importamos tus algoritmos

using namespace std;

void ejecutar_oraculo(int iteraciones) {
    srand(time(NULL));
    int aciertos = 0;

    for (int i = 0; i < iteraciones; ++i) {
        // 1. GENERAR DATOS ALEATORIOS
        // Usamos un M pequeño (entre 4 y 15) para que las versiones Sin Poda no tarden horas.
        int M = rand() % 12 + 4; 
        
        // Supongamos 5 tallas de camisetas (0 a 4)
        vector<int> disp_original(5);
        for (int k = 0; k < 5; ++k) {
            disp_original[k] = rand() % (M / 2 + 1); // Inventario aleatorio
        }

        // Preferencias de los M voluntarios
        vector<pair<int, int>> v(M);
        for (int j = 0; j < M; ++j) {
            v[j].first = rand() % 5;
            v[j].second = rand() % 5;
        }

        // 2. PREPARAR COPIAS (Vital porque tus funciones modifican 'disp' y 's')
        vector<int> disp1 = disp_original;
        vector<int> disp2 = disp_original;
        vector<int> disp3 = disp_original;
        vector<int> disp4 = disp_original;

        vector<int> s1(M, -1);
        vector<int> s2(M, -1);

        // 3. EJECUTAR LAS 4 VERSIONES
        bool res_rec_poda   = rec_poda(0, s1, disp1, v, M);
        bool res_rec_nopoda = rec_nopoda(0, s2, disp2, v, M);
        bool res_it_poda    = it_poda(M, v, disp3);
        bool res_it_nopoda  = it_nopoda(M, v, disp4);

        // 4. COMPARAR RESULTADOS
        // Todas deben devolver exactamente el mismo valor booleano (si se pudo o no)
        assert(res_rec_poda == res_rec_nopoda && "Fallo: Recursivo Poda difiere de Recursivo Sin Poda");
        assert(res_rec_nopoda == res_it_poda && "Fallo: Recursivo Sin Poda difiere de Iterativo Poda");
        assert(res_it_poda == res_it_nopoda && "Fallo: Iterativo Poda difiere de Iterativo Sin Poda");

        aciertos++;
    }

    cout << "==================================================" << endl;
    cout << "  ORACULO BACKTRACKING COMPLETADO CON EXITO       " << endl;
    cout << "==================================================" << endl;
    cout << "Casos aleatorios probados: " << aciertos << "/" << iteraciones << endl;
    cout << "Las 4 implementaciones devuelven el mismo resultado." << endl;
    cout << "La logica de poda es segura y correcta." << endl;
    cout << "==================================================" << endl;
}

int main() {
    cout << "Iniciando Test Oraculo para Backtracking..." << endl;
    cout << "(Ten paciencia, las versiones sin poda pueden ser algo lentas...)" << endl;
    
    // Probamos con 40 casos aleatorios.
    ejecutar_oraculo(40); 
    
    return 0;
}