// ==========================================
// tests_unitarios_BT.cpp
// ==========================================

#include <iostream>
#include <vector>
#include <utility>
#include "reparto_camisetas_BT.h" 

using namespace std;

// Función auxiliar para ejecutar un caso de prueba en los 4 algoritmos
void ejecutar_test(int num_test, const string& descripcion, int M, const vector<pair<int, int>>& v, const vector<int>& disp_inicial, bool esperado) {
    cout << "--- TEST " << num_test << ": " << descripcion << " ---" << endl;
    
    // 1. Recursivo Con Poda
    vector<int> disp1 = disp_inicial;
    vector<int> s1(M, -1);
    bool res_rec_poda = rec_poda(0, s1, disp1, v, M);
    
    // 2. Recursivo Sin Poda
    vector<int> disp2 = disp_inicial;
    vector<int> s2(M, -1);
    bool res_rec_nopoda = rec_nopoda(0, s2, disp2, v, M);
    
    // 3. Iterativo Con Poda
    vector<int> disp3 = disp_inicial;
    bool res_it_poda = it_poda(M, v, disp3);
    
    // 4. Iterativo Sin Poda
    vector<int> disp4 = disp_inicial;
    bool res_it_nopoda = it_nopoda(M, v, disp4);

    // Verificamos si todos han dado el resultado correcto
    bool todos_ok = (res_rec_poda == esperado) && 
                    (res_rec_nopoda == esperado) && 
                    (res_it_poda == esperado) && 
                    (res_it_nopoda == esperado);

    if (todos_ok) {
        cout << "[OK] Los 4 algoritmos coinciden. Resultado: " << (esperado ? "YES" : "NO") << "\n\n";
    } else {
        cout << "[ERROR] Discrepancia encontrada en los resultados:\n";
        cout << "  - Esperado:   " << (esperado ? "YES" : "NO") << "\n";
        cout << "  - Rec Poda:   " << (res_rec_poda ? "YES" : "NO") << "\n";
        cout << "  - Rec NoPoda: " << (res_rec_nopoda ? "YES" : "NO") << "\n";
        cout << "  - It Poda:    " << (res_it_poda ? "YES" : "NO") << "\n";
        cout << "  - It NoPoda:  " << (res_it_nopoda ? "YES" : "NO") << "\n\n";
    }
}

int main() {
    cout << "=================================================\n";
    cout << "   BATERIA DE TESTS UNITARIOS - BACKTRACKING\n";
    cout << "=================================================\n\n";

    // Recordatorio de índices: XXL=0, XL=1, L=2, M=3, S=4, XS=5

    // ---------------------------------------------------------
    // TEST 1: Ejemplo 1 del Mooshak (Tiene solución)
    // N=18 (3 de cada), M=6
    // Voluntarios: L XL, XL L, XXL XL, S XS, M S, M L
    // ---------------------------------------------------------
    vector<int> disp1(6, 18 / 6); 
    vector<pair<int, int>> v1 = {{2,1}, {1,2}, {0,1}, {4,5}, {3,4}, {3,2}};
    ejecutar_test(1, "Ejemplo oficial 1 (Con Solucion)", 6, v1, disp1, true);

    // ---------------------------------------------------------
    // TEST 2: Ejemplo 2 del Mooshak (No tiene solución)
    // N=6 (1 de cada), M=4
    // Voluntarios: S XL, L S, L XL, L XL
    // ---------------------------------------------------------
    vector<int> disp2(6, 6 / 6); 
    vector<pair<int, int>> v2 = {{4,1}, {2,4}, {2,1}, {2,1}};
    ejecutar_test(2, "Ejemplo oficial 2 (Falta stock)", 4, v2, disp2, false);

    // ---------------------------------------------------------
    // TEST 3: Ejemplo 3 del Mooshak (Caso mínimo)
    // N=6 (1 de cada), M=1
    // Voluntarios: L M
    // ---------------------------------------------------------
    vector<int> disp3(6, 6 / 6);
    vector<pair<int, int>> v3 = {{2,3}};
    ejecutar_test(3, "Ejemplo oficial 3 (Caso minimo)", 1, v3, disp3, true);
    
    // ---------------------------------------------------------
    // TEST 4: Caso Extremo (Inventario vacío)
    // N=0, M=1
    // ---------------------------------------------------------
    vector<int> disp4(6, 0);
    vector<pair<int, int>> v4 = {{0,1}};
    ejecutar_test(4, "Inventario vacio", 1, v4, disp4, false);

    cout << "=================================================\n";
    cout << "             TESTS FINALIZADOS\n";
    cout << "=================================================\n";
    
    return 0;
}