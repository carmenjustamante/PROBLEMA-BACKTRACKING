#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

/**
 * Mapea el nombre de la talla a un índice del vector de disponibilidad (0 a 5).
 */
int talla_a_indice(const string& talla) {
    if (talla == "XXL") return 0; 
    if (talla == "XL") return 1;
    if (talla == "L") return 2;   
    if (talla == "M") return 3;
    if (talla == "S") return 4;   
    return 5; // XS
}

/**
 * Determina si hemos llegado a una solución válida:
 * Estamos en el último voluntario y el stock de tallas no es negativo.
 */
bool solucion(int nivel, const vector<int>& disponibles, int M) {
    if (nivel != M - 1) return false;
    for (int d : disponibles) if (d < 0) return false;
    return true;
}

/**
 * PODA: Si al asignar una talla el stock de esa talla baja de 0, 
 * dejamos de explorar esa rama porque ya es una solución inválida.
 */
bool criterio(int nivel, const vector<int>& disponibles, int M) {
    for (int d : disponibles) if (d < 0) return false; // Poda activa
    return nivel < M - 1;
}

/**
 * Verifica si el voluntario actual tiene más opciones de talla por probar.
 * s[nivel] < 1 indica que aún podemos probar la segunda opción (índice 1).
 */
bool masHermanos(int nivel, const vector<int>& s) {
    return s[nivel] < 1;
}

/**
 * Genera el siguiente estado de asignación para el voluntario en el nivel actual.
 * Actualiza dinámicamente el vector de disponibilidad de camisetas.
 */
void generar(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    int oldValue = s[nivel];
    s[nivel]++; // Pasa de -1 a 0 (1ª opción) o de 0 a 1 (2ª opción)
    
    // Si saltamos de la 1ª a la 2ª opción, liberamos la 1ª en el stock
    if (oldValue == 0 && s[nivel] == 1) {
        disponibles[voluntarios[nivel].first]++;
    }
    
    // Restamos del stock la talla que el voluntario está intentando usar
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]--;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]--;
    }
}

/**
 * Deshace la asignación actual para dejar el sistema listo para el backtracking.
 */
void retroceder(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]++;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]++;
    }
    s[nivel] = -1; // Limpiamos la elección
}

// --- MOTOR RECURSIVO ---

/**
 * Función recursiva que explora las combinaciones de reparto.
 * nivel: representa al voluntario actual procesándose.
 */
bool repartir_rec(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int,int>>& v, int M) {
    bool exito = false;
    
    // Mientras no hayamos encontrado solución y queden tallas por probar para este voluntario
    while (!exito && masHermanos(nivel, s)) {
        generar(nivel, s, disp, v);
        
        if (solucion(nivel, disp, M)) {
            exito = true; // Hemos completado el reparto con éxito
        } 
        else if (criterio(nivel, disp, M)) {
            // Si la rama es prometedora, bajamos al siguiente voluntario (recursión)
            exito = repartir_rec(nivel + 1, s, disp, v, M);
        }
    }
    
    // Si después de probar sus opciones no hubo éxito, limpiamos antes de subir de nivel
    if (!exito) retroceder(nivel, s, disp, v);
    
    return exito;
}

int main() {
    int t; 
    if (!(cin >> t)) return 0;
    
    while (t--) {
        int n, m; 
        cin >> n >> m;
        
        vector<pair<int, int>> v(m);
        for (int i = 0; i < m; ++i) {
            string t1, t2; 
            cin >> t1 >> t2;
            v[i] = {talla_a_indice(t1), talla_a_indice(t2)};
        }
        
        // Inicializamos: s con -1 (sin elegir) y disp con n/6 para cada una de las 6 tallas
        vector<int> s(m, -1), disp(6, n / 6);
        
        // Ejecutamos el algoritmo recursivo desde el voluntario 0
        if (m > 0 && repartir_rec(0, s, disp, v, m)) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}