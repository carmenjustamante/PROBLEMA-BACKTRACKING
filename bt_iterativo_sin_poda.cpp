#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

// Función para convertir el string de talla en un índice de 0 a 5
int talla_a_indice(const string& talla) {
    if (talla == "XXL") return 0; 
    if (talla == "XL") return 1;
    if (talla == "L") return 2;   
    if (talla == "M") return 3;
    if (talla == "S") return 4;   
    return 5; // XS
}

// Comprueba si hemos asignado una camiseta a cada voluntario (nivel M-1)
// y si al final la configuración es válida (ningún stock negativo)
bool solucion(int nivel, const vector<int>& disponibles, int M) {
    if (nivel != M - 1) return false;
    for (int d : disponibles) if (d < 0) return false;
    return true;
}

bool criterio(int nivel, const vector<int>& disponibles, int M) {
    // SIN PODA: aquí NO se comprueba 
    // si 'disponibles' tiene valores negativos. El algoritmo seguirá 
    // bajando niveles aunque ya no queden camisetas, perdiendo eficiencia.
    return nivel < M - 1;
}

// Verifica si el voluntario actual tiene una segunda opción (índice 1) por probar
bool masHermanos(int nivel, const vector<int>& s) {
    return s[nivel] < 1;
}

// Realiza la asignación de la talla y actualiza el stock disponible
void generar(int nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    int oldValue = s[nivel];
    s[nivel]++; // Incrementa la opción (de -1 a 0, o de 0 a 1)
    
    // Si cambiamos de la primera a la segunda opción, devolvemos la primera al stock
    if (oldValue == 0 && s[nivel] == 1) {
        disponibles[voluntarios[nivel].first]++;
    }
    
    // Restamos del stock la talla elegida actualmente
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]--;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]--;
    }
}

// Deshace la asignación para volver atrás en el árbol de decisión (Backtracking)
void retroceder(int& nivel, vector<int>& s, vector<int>& disponibles, const vector<pair<int, int>>& voluntarios) {
    if (s[nivel] == 0) {
        disponibles[voluntarios[nivel].first]++;
    } else if (s[nivel] == 1) {
        disponibles[voluntarios[nivel].second]++;
    }
    s[nivel] = -1; // Reset del estado del voluntario
    --nivel;       // Sube un nivel
}

// Función principal de lógica: intenta repartir las camisetas de forma iterativa
bool repartir(int M, const vector<pair<int, int>>& voluntarios, vector<int>& disponibles) {
    if (M == 0) return true;
    vector<int> s(M, -1); // Vector de soluciones parciales
    int nivel = 0;
    
    while (nivel != -1) {
        generar(nivel, s, disponibles, voluntarios);
        
        // Si encontramos una combinación válida para todos, terminamos
        if (solucion(nivel, disponibles, M)) return true;
        
        // Si podemos seguir bajando (aunque el stock sea negativo, por falta de poda)
        if (criterio(nivel, disponibles, M)) {
            ++nivel;
        } else {
            // Si no hay más niveles o hemos terminado las opciones de este nivel, retrocedemos
            while (nivel >= 0 && !masHermanos(nivel, s)) {
                retroceder(nivel, s, disponibles, voluntarios);
            }
        }
    }
    return false;
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
            // Guardamos las dos tallas preferidas de cada voluntario como índices
            v[i] = {talla_a_indice(t1), talla_a_indice(t2)};
        }
        
        // El stock inicial de cada talla es N/6
        vector<int> disp(6, n / 6);
        
        if (repartir(m, v, disp)) cout << "YES" << endl;
        else cout << "NO" << endl;
    }
    return 0;
}