# PROBLEMA-BACKTRACKING
SOLUCION DE UN PROBLEMA UTILIZANDO BACKTRACKING Y ANÁLISIS
Contexto
Nuestro amigo Victor participa como un instructor en un programa de voluntariado. El jefe de Victor
le ha pedido que distribuya N camisetas entre M voluntarios, una camiseta para cada voluntario, donde N
es múltiplo de seis, y N ≥ M . Hay el mismo número de camisetas de cada una de las 6 tallas disponibles:
XXL, XL, L, M, S, y XS. Victor tiene un pequeño problema porque a cada voluntario sólo le vienen dos
tallas de camiseta y no sabe si tendrá las tallas adecuadas para todos.
Problema
El problema consiste en escribir un programa que decida si Victor puede distribuir las camisetas de
tal modo que todos los voluntarios tengan una camiseta que les venga bien. Téngase en cuenta que si
N > M pueden sobrar algunas camisetas.
Descripción de los algoritmos implementadosEl objetivo de este trabajo es resolver el problema de asignación de camisetas mediante técnicas de Backtracking, explorando un espacio de búsqueda binario donde cada nodo representa la decisión de asignar una de las dos tallas preferidas a un voluntario. Se han desarrollado cuatro variantes del algoritmo —iterativo y recursivo, ambos con y sin poda— para analizar cómo influyen la estructura de control y la optimización de restricciones en el rendimiento del sistema. Mientras que las versiones sin poda realizan una exploración exhaustiva de todas las combinaciones posibles ($O(2^M)$), las versiones con poda incorporan una función de criterio que verifica la disponibilidad de stock en tiempo real, permitiendo abortar ramas inviables de forma temprana. Esta comparativa nos permite observar que, aunque la complejidad teórica en el peor caso se mantiene, la poda y la gestión eficiente de la memoria (especialmente en la versión iterativa) son fundamentales para transformar un problema de crecimiento exponencial en uno tratable para casos prácticos.Detalles técnicos clave incluidos:Justificación de las variantes: Se explica que es para comparar el rendimiento y la gestión de memoria.Uso de la Poda: Se destaca como la herramienta para optimizar el árbol de búsqueda.Complejidad: Se menciona el orden exponencial $O(2^M)$ y cómo la poda ayuda en la práctica.Estructuras: Se diferencia entre la recursión (pila del sistema) y la iteración.
Cómo utilizar el proyecto
Para facilitar la gestión del código, la compilación y la generación de los resultados experimentales, se
ha elaborado un fichero Makefile que describe cómo se compila el proyecto y automatiza la construcción
de los ejecutables.
8
Compilación y Validación
Para compilar todas las herramientas de Backtracking aplicando banderas de máxima optimización
(-O3), se debe abrir una terminal en el directorio del proyecto y ejecutar el siguiente comando:
mingw32-make -f Makefile_BT
Este proceso generará los ejecutables tests_unitarios_BT.exe y medir_tiempos.exe. Antes de
proceder con el estudio de eficiencia, se recomienda validar que la lógica de exploración y poda funciona
correctamente ejecutando la batería de pruebas:
.\tests_unitarios_BT.exe
Generación del estudio de tiempos
Una vez validado y compilado el código, el proceso para obtener los datos empíricos y las gráficas
consta de dos pasos:
1. Ejecución del medidor: Se lanza el programa compilado encargado de explorar los árboles de
decisión para distintos valores de entrada. Tras su ejecución, los tiempos medidos se guardarán en
el archivo resultados.csv.
.\medir_tiempos.exe
2. Generación de las gráficas (requiere Python): Mediante el script de análisis, se procesa el
archivo CSV para generar los archivos de imagen (PNG) que muestran el comportamiento del
algoritmo en sus diferentes casos (tM y tm), así como el impacto de las podas.
python regresion_BT.py
