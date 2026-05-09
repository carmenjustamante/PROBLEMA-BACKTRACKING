import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import linregress

# Cargar datos
df = pd.read_csv('resultados.csv')
df = df.replace(0, 0.0001)
m = df['M']

def plot_graph(title, cols, labels, filename, log=False):
    plt.figure()
    for col, lab in zip(cols, labels):
        plt.plot(m, df[col], marker='o', label=lab)

    plt.title(title)
    plt.xlabel('Número de voluntarios (M)')
    plt.ylabel('Tiempo (microsegundos)')

    if log:
        plt.yscale('log')

    plt.legend()
    plt.grid(True)
    plt.savefig(filename)
    plt.close()

# ============================
# GRÁFICAS CLAVE
# ============================

# 1. PEOR CASO (LO IMPORTANTE)
plot_graph("Recursivo - Peor caso (LOG)",
           ["RecPoda_Peor", "RecNoPoda_Peor"],
           ["Con poda", "Sin poda"],
           "1_rec_peor_log.png", log=True)

plot_graph("Iterativo - Peor caso (LOG)",
           ["ItPoda_Peor", "ItNoPoda_Peor"],
           ["Con poda", "Sin poda"],
           "2_it_peor_log.png", log=True)

# 2. COMPARACIÓN GLOBAL
plot_graph("Comparación total (LOG)",
           ["RecNoPoda_Peor", "RecPoda_Peor", "ItNoPoda_Peor", "ItPoda_Peor"],
           ["Rec NoPoda", "Rec Poda", "It NoPoda", "It Poda"],
           "3_total_log.png", log=True)

# 3. TODOS LOS CASOS
plot_graph("Recursivo - Todos los casos",
           ["RecPoda_Mejor","RecPoda_Medio","RecPoda_Peor"],
           ["Mejor","Medio","Peor"],
           "4_rec_casos.png", log=True)

plot_graph("Iterativo - Todos los casos",
           ["ItPoda_Mejor","ItPoda_Medio","ItPoda_Peor"],
           ["Mejor","Medio","Peor"],
           "5_it_casos.png", log=True)

# ============================
# GRÁFICAS EXTRA 
# ============================

# 6 y 7. ESCALA LINEAL (EL "EFECTO MURO")
plot_graph("Recursivo - Peor caso (LINEAL)",
           ["RecPoda_Peor", "RecNoPoda_Peor"],
           ["Con poda", "Sin poda"],
           "6_rec_peor_lineal.png", log=False)

plot_graph("Iterativo - Peor caso (LINEAL)",
           ["ItPoda_Peor", "ItNoPoda_Peor"],
           ["Con poda", "Sin poda"],
           "7_it_peor_lineal.png", log=False)

# 8. EL PESO DE LA RECURSIVIDAD (Rec vs It)
plot_graph("Coste de la Recursividad (Sin Poda)",
           ["RecNoPoda_Peor", "ItNoPoda_Peor"],
           ["Recursivo a lo bruto", "Iterativo a lo bruto"],
           "8_recursivo_vs_iterativo.png", log=True)

# 9. GRÁFICO DE BARRAS (FOTO FINISH PARA EL M MÁS GRANDE)
plt.figure()
# Cogemos la última fila de datos (el M más grande que hayas medido)
ultima_fila = df.iloc[-1]
M_max = int(ultima_fila['M'])

tiempos_finales = [
    ultima_fila['ItPoda_Peor'], 
    ultima_fila['RecPoda_Peor'], 
    ultima_fila['ItNoPoda_Peor'], 
    ultima_fila['RecNoPoda_Peor']
]
etiquetas = ['It. Poda', 'Rec. Poda', 'It. SinPoda', 'Rec. SinPoda']
colores = ['green', 'orange', 'red', 'purple']

plt.bar(etiquetas, tiempos_finales, color=colores)
plt.title(f'Comparativa final absoluta para M = {M_max}')
plt.ylabel('Tiempo (microsegundos) - Escala LOG')
plt.yscale('log')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig("9_barras_comparativa.png")
plt.close()

# ============================
# 10. CONTRASTE TEÓRICO-EXPERIMENTAL (REGRESIONES)
# ============================
m_vals = m.values
x_exp = 2.0 ** m_vals  # Transformación exponencial para versiones Sin Poda
x_lin = m_vals         # Transformación lineal para versiones Con Poda

def plot_regression(x_data, y_data, title, filename):
    res = linregress(x_data, y_data)
    r2 = res.rvalue ** 2
    pred = res.intercept + res.slope * x_data

    plt.figure(figsize=(8, 5))
    plt.plot(m_vals, y_data, 'x', color='blue', label='Experimental')
    # Etiqueta adaptada según el ajuste
    ajuste_lbl = "Ajuste O(2^M)" if "Sin Poda" in title else "Ajuste O(M)"
    plt.plot(m_vals, pred, '-', color='orange', label=f'{ajuste_lbl} | R² = {r2:.4f}')
    
    plt.title(title)
    plt.xlabel('Número de voluntarios (M)')
    plt.ylabel('Tiempo (microsegundos)')
    plt.legend()
    plt.grid(True)
    plt.savefig(filename)
    plt.close()
    return r2

r2_rnp = plot_regression(x_exp, df['RecNoPoda_Peor'], "Contraste: Recursivo Sin Poda", "10_contraste_rec_nopoda.png")
r2_inp = plot_regression(x_exp, df['ItNoPoda_Peor'], "Contraste: Iterativo Sin Poda", "11_contraste_it_nopoda.png")
r2_rp = plot_regression(x_lin, df['RecPoda_Peor'], "Contraste: Recursivo Con Poda", "12_contraste_rec_poda.png")
r2_ip = plot_regression(x_lin, df['ItPoda_Peor'], "Contraste: Iterativo Con Poda", "13_contraste_it_poda.png")

print("\n--- RESULTADOS DE LAS REGRESIONES BT ---")
print(f"R^2 Rec Sin Poda (Ajuste 2^M): {r2_rnp:.4f}")
print(f"R^2 It Sin Poda  (Ajuste 2^M): {r2_inp:.4f}")
print(f"R^2 Rec Con Poda (Ajuste M):   {r2_rp:.4f}")
print(f"R^2 It Con Poda  (Ajuste M):   {r2_ip:.4f}")
print("----------------------------------------\n")
print("GRÁFICAS PERFECTAS GENERADAS")