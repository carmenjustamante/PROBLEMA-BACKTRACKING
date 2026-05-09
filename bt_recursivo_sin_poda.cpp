#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

// Mapeo de tallas a índices
int talla_a_indice(const string& talla) {
    if (talla == "XXL") return 0; 
    if (talla == "XL") return 1;
    if (talla == "L") return 2;   
    if (talla == "M") return 3;
    if (talla == "S") return 4;   
    return 5;
}

/**
 * Función de comprobación final.
 * Solo aquí se verifica si el stock es válido (ninguno negativo).
 */
bool solucion(int nivel, const vector<int>& disponibles, int M) {
    if (nivel != M - 1) return false;
    for (int d : disponibles) if (d < 0) return false;
    return true;
}

/**
 * CRITERIO SIN PODA:
 * No comprueba si nos hemos quedado sin camisetas (disponibles < 0).
 * Simplemente permite seguir bajando niveles hasta el último voluntario.
 */
bool criterio(int nivel, const vector<int>& disponibles, int M) {
    return nivel < M - 1; 
}

// Verifica si el voluntario actual puede probar su segunda opción
bool masHermanos(int nivel, const vector<int>& s) {
    return s[nivel] < 1;
}

// Realiza la asignación y modifica el stock disponible
void generar(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    int oldValue = s[nivel];
    s[nivel]++;
    
    // Si saltamos de la opción 1 a la 2, restauramos la opción 1
    if (oldValue == 0 && s[nivel] == 1) {
        disponibles[voluntarios[nivel].first]++;
    }
    
    // Descontamos la talla elegida actualmente
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]--;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]--;
    }
}

// Deshace la asignación para el backtracking
void retroceder(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]++;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]++;
    }
    s[nivel] = -1;
}

/**
 * MOTOR RECURSIVO (Fuerza Bruta)
 * Explora el árbol de decisiones mediante llamadas recursivas.
 * Al no tener poda en 'criterio', el rendimiento será pobre en casos grandes.
 */
bool repartir_rec(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int,int>>& v, int M) {
    bool exito = false;
    
    while (!exito && masHermanos(nivel, s)) {
        generar(nivel, s, disp, v);
        
        // Verifica si hemos llegado a una solución completa y válida
        if (solucion(nivel, disp, M)) {
            exito = true;
        } 
        // Si no es solución, sigue bajando (sin importar si el stock es negativo)
        else if (criterio(nivel, disp, M)) {
            exito = repartir_rec(nivel + 1, s, disp, v, M);
        }
    }
    
    // Si la rama no llevó al éxito, limpiamos el estado antes de retornar
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
        
        // s: historial de decisiones, disp: stock inicial (n/6 por cada talla)
        vector<int> s(m, -1), disp(6, n / 6);
        
        if (m > 0 && repartir_rec(0, s, disp, v, m)) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}