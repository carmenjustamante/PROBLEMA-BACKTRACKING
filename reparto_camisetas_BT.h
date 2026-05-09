#ifndef REPARTO_CAMISETAS_BT_H
#define REPARTO_CAMISETAS_BT_H

#include <vector>
#include <utility>

using namespace std;

//FUNCIONES DE APOYO COMUNES

inline bool es_solucion(int nivel, int M) {
    return (nivel == M - 1);
}

inline bool tiene_hermanos(int nivel, const vector<int>& s) {
    return s[nivel] < 1;
}

inline void generar_hijo(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int, int>>& v) {
    int ant = s[nivel];
    s[nivel]++;
    if (ant == -1) { 
        disp[v[nivel].first]--;
    } else { 
        disp[v[nivel].first]++;
        disp[v[nivel].second]--;
    }
}

inline void retroceder_estado(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int, int>>& v) {
    if (s[nivel] == 0) disp[v[nivel].first]++;
    else if (s[nivel] == 1) disp[v[nivel].second]++;
    s[nivel] = -1;
}

// --- 1. RECURSIVO CON PODA ---
bool rec_poda(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int,int>>& v, int M) {
    bool exito = false;
    while (!exito && tiene_hermanos(nivel, s)) {
        generar_hijo(nivel, s, disp, v);
        bool viable = true;
        for(int d : disp) if(d < 0) { viable = false; break; }

        if (es_solucion(nivel, M) && viable) exito = true; // Actualizado
        else if (viable && nivel < M - 1) {
            exito = rec_poda(nivel + 1, s, disp, v, M);
        }
    }
    if (!exito) retroceder_estado(nivel, s, disp, v);
    return exito;
}

// --- 2. RECURSIVO SIN PODA ---
bool rec_nopoda(int nivel, vector<int>& s, vector<int>& disp, const vector<pair<int,int>>& v, int M) {
    bool exito = false;
    while (!exito && tiene_hermanos(nivel, s)) {
        generar_hijo(nivel, s, disp, v);
        if (es_solucion(nivel, M)) { // Actualizado
            bool valido = true;
            for(int d : disp) if(d < 0) { valido = false; break; }
            if (valido) exito = true;
        }
        else if (nivel < M - 1) {
            exito = rec_nopoda(nivel + 1, s, disp, v, M);
        }
    }
    if (!exito) retroceder_estado(nivel, s, disp, v);
    return exito;
}

// --- 3. ITERATIVO CON PODA ---
bool it_poda(int M, const vector<pair<int,int>>& v, vector<int>& disp) {
    if (M == 0) return true;
    vector<int> s(M, -1);
    int nivel = 0;
    while (nivel >= 0) {
        generar_hijo(nivel, s, disp, v);
        bool viable = true;
        for(int d : disp) if(d < 0) { viable = false; break; }

        if (es_solucion(nivel, M) && viable) return true; // Actualizado
        
        if (viable && nivel < M - 1) {
            nivel++;
        } else {
            while (nivel >= 0 && !tiene_hermanos(nivel, s)) {
                retroceder_estado(nivel, s, disp, v);
                nivel--;
            }
        }
    }
    return false;
}

// --- 4. ITERATIVO SIN PODA ---
bool it_nopoda(int M, const vector<pair<int,int>>& v, vector<int>& disp) {
    if (M == 0) return true;
    vector<int> s(M, -1);
    int nivel = 0;
    while (nivel >= 0) {
        generar_hijo(nivel, s, disp, v);
        if (es_solucion(nivel, M)) { // Actualizado
            bool valido = true;
            for(int d : disp) if(d < 0) { valido = false; break; }
            if (valido) return true;
        }
        if (nivel < M - 1) {
            nivel++;
        } else {
            while (nivel >= 0 && !tiene_hermanos(nivel, s)) {
                retroceder_estado(nivel, s, disp, v);
                nivel--;
            }
        }
    }
    return false;
}

#endif