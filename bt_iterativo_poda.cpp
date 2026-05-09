#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

/**
 * Convierte el nombre de la talla en un índice numérico (0-5).
 * Facilita el manejo de los recursos en un vector de disponibilidad.
 */
int talla_a_indice(const string& talla) {
    if (talla == "XXL") return 0; 
    if (talla == "XL") return 1;
    if (talla == "L") return 2;   
    if (talla == "M") return 3;
    if (talla == "S") return 4;   
    return 5; // XS
}

// --- FUNCIONES GENÉRICAS DE BACKTRACKING ---

/**
 * Comprueba si hemos llegado a una solución válida.
 * Debe estar en el último nivel y no haber agotado existencias (disponibles < 0).
 */
bool solucion(int nivel, const vector<int>& disponibles, int M) {
    if (nivel != M - 1) return false;
    for (int d : disponibles) if (d < 0) return false;
    return true;
}

/**
 * Determina si el camino actual es prometedor (Poda).
 * Si alguna talla tiene stock negativo, dejamos de explorar esa rama.
 */
bool criterio(int nivel, const vector<int>& disponibles, int M) {
    for (int d : disponibles) if (d < 0) return false; 
    return nivel < M - 1; // Permite seguir bajando niveles si no hemos terminado
}

/**
 * Indica si el voluntario actual todavía tiene otra opción de talla por probar.
 * s[nivel] == 0 es la primera opción, s[nivel] == 1 es la segunda.
 */
bool masHermanos(int nivel, const vector<int>& s) {
    return s[nivel] < 1;
}

/**
 * Genera el siguiente estado (intenta asignar la siguiente talla disponible al voluntario).
 * Actualiza el vector de 'disponibles' restando stock de la talla elegida.
 */
void generar(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    int oldValue = s[nivel];
    s[nivel]++; // Pasa de -1 a 0 (1ª opción) o de 0 a 1 (2ª opción)
    
    // Si saltamos de la 1ª a la 2ª opción, devolvemos la 1ª al stock
    if (oldValue == 0 && s[nivel] == 1) {
        disponibles[voluntarios[nivel].first]++;
    }
    
    // Restamos del stock la talla que estamos probando actualmente
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]--;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]--;
    }
}

/**
 * Deshace los cambios realizados en el nivel actual para volver al nivel anterior.
 * Restaura el stock de la talla que estaba ocupando el voluntario.
 */
void retroceder(int& nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]++;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]++;
    }
    s[nivel] = -1; // Reiniciamos el estado del voluntario
    --nivel;       // Subimos un nivel en el árbol
}

// --- MOTOR DE BÚSQUEDA ---

/**
 * Intenta repartir las camisetas entre M voluntarios usando backtracking iterativo.
 */
bool repartir(int M, const vector<pair<int, int>>& voluntarios, vector<int>& disponibles) {
    if (M == 0) return true;
    
    vector<int> s(M, -1); // Estado de cada voluntario (-1: sin asignar, 0: 1ª talla, 1: 2ª talla)
    int nivel = 0;
    
    while (nivel != -1) {
        generar(nivel, s, disponibles, voluntarios);
        
        if (solucion(nivel, disponibles, M)) return true; // ¡Éxito!
        
        if (criterio(nivel, disponibles, M)) {
            ++nivel; // Avanzamos al siguiente voluntario
        } else {
            // Si no hay solución por aquí, retrocedemos hasta encontrar un voluntario con opciones pendientes
            while (nivel >= 0 && !masHermanos(nivel, s)) {
                retroceder(nivel, s, disponibles, voluntarios);
            }
        }
    }
    return false; // No se encontró una combinación válida
}

int main() {
    int t; 
    if (!(cin >> t)) return 0; // Número de casos de prueba
    
    while (t--) {
        int n, m; 
        cin >> n >> m; // n: total camisetas, m: voluntarios
        
        vector<pair<int, int>> v(m);
        for (int i = 0; i < m; ++i) {
            string t1, t2; 
            cin >> t1 >> t2;
            v[i] = {talla_a_indice(t1), talla_a_indice(t2)};
        }
        
        // Cada una de las 6 tallas tiene n/6 unidades disponibles
        vector<int> disp(6, n / 6);
        
        if (repartir(m, v, disp)) cout << "YES" << endl;
        else cout << "NO" << endl;
    }
    return 0;
}