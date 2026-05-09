// ============================
// medir_tiempos.cpp (VERSIÓN FINAL BUENA)
// ============================
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include "reparto_camisetas_BT.h"

using namespace std;
using namespace std::chrono;

// CASOS BIEN DEFINIDOS
void generar_caso(int M, vector<pair<int,int>>& v, vector<int>& disp, int tipoCaso) {
    v.resize(M);
    disp.assign(6, 0);

    if (tipoCaso == 0) { 
        // MEJOR CASO: solución inmediata
        for(int i=0;i<M;i++) v[i] = {i % 6, (i+1) % 6};
        disp.assign(6, M);
    }
    else if (tipoCaso == 1) {
        // PEOR CASO: SIN SOLUCIÓN (explora todo)
        for(int i=0;i<M;i++) v[i] = {0,1};
        disp.assign(6, 1);
    }
    else {
        // CASO INTERMEDIO
        for(int i=0;i<M;i++) v[i] = {i%3, (i+2)%6};
        disp.assign(6, M/2);
    }
}


double cronometrar(int M, int tipoAlg, int tipoCaso) {
    vector<pair<int,int>> v;
    vector<int> disp;

    generar_caso(M, v, disp, tipoCaso);

    int reps = 10;

    auto start = high_resolution_clock::now();

    for(int i=0;i<reps;i++){
        vector<int> disp_copy = disp;
        vector<int> s(M, -1);

        if (tipoAlg == 0) rec_poda(0, s, disp_copy, v, M);
        else if (tipoAlg == 1) rec_nopoda(0, s, disp_copy, v, M);
        else if (tipoAlg == 2) it_poda(M, v, disp_copy);
        else it_nopoda(M, v, disp_copy);
    }

    auto stop = high_resolution_clock::now();

    return duration_cast<microseconds>(stop - start).count() / (double)reps;
}

int main(){
    ofstream csv("resultados.csv");

    csv << "M,RecPoda_Mejor,RecPoda_Peor,RecPoda_Medio,RecNoPoda_Mejor,RecNoPoda_Peor,RecNoPoda_Medio,ItPoda_Mejor,ItPoda_Peor,ItPoda_Medio,ItNoPoda_Mejor,ItNoPoda_Peor,ItNoPoda_Medio\n";

    for(int M=4; M<=30; M+=2){
        cout << "M=" << M << endl;

        csv << M << ","

        // Recursivo poda
        << cronometrar(M,0,0) << "," << cronometrar(M,0,1) << "," << cronometrar(M,0,2) << ","

        // Recursivo no poda
        << cronometrar(M,1,0) << "," << cronometrar(M,1,1) << "," << cronometrar(M,1,2) << ","

        // Iterativo poda
        << cronometrar(M,2,0) << "," << cronometrar(M,2,1) << "," << cronometrar(M,2,2) << ","

        // Iterativo no poda
        << cronometrar(M,3,0) << "," << cronometrar(M,3,1) << "," << cronometrar(M,3,2)

        << "\n";
    }

    csv.close();
    return 0;
}



