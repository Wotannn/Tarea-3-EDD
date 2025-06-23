# Tarea 3 EDD

Integrantes:
Daniel Prieto 202473087-8
Benjamín Ríos 202473081-9


Análisis de Complejidad Temporal para la Función correrUber()

Este análisis se centra en la complejidad de la función correrUber(), utilizando la notación adecuada para describir cómo el tiempo de ejecución crece con el tamaño de las entradas.

La Complejidad General de la función correrUber es: O(S⋅C⋅(V+E))

Justificación

La función correrUber() puede ser dividida en 3 etapas:


1. Fase de carga de datos

Al llamar a la función correrUber(), esta recibe el string nombreArchivo, string con el que se llama a la función leerArchivo, de donde al leer el archivo, se obtiene V(cantidad de nodos), E(cantidad de caminos) y C(cantidad de conductores), esto, junto con la apertura del archivo, son operaciones de tiempo de ejecución constante O(1).

Constructor tGrafo(nNodos): Dentro del constructor de la clase tGrafo, se inicializa el arreglo de punteros "adyacencias" con un bucle que recorre hasta cantidadNodos + 1 elementos. Esto toma O(V) tiempo.

Bucle para cargar aristas (for (int i = 0; i < nArcos; i++)): Este bucle se ejecuta E veces. Dentro del bucle, la llamada a grafo->setEdge(origen, destino) es una operación con tiempo constante, O(1), ya que solo crea un nuevo nodo y lo inserta a la lista de adyacencia. Es por esto que este bloque es O(E).

Bucle para cargar conductores: Este bucle se ejecuta C veces, y dentro del bucle, la creación de una instancia Conductor es una operación de tiempo constante, O(1). Por lo tanto, este bloque es O(C).

Complejidad total de leerArchivo: Sumando estos, la complejidad es O(1)+O(V)+O(E)+O(C)=O(V+E+C). 

Dado que el número de conductores (C) no puede exceder el número de nodos (V) donde se ubican, la complejidad total se puede simplificar a O(V+E).



2. Fase de solicitudes de viaje

Esta es la fase principal y se repite por cada solicitud de viaje. Se puede definir también una variable S, que será el número de solicitudes de viaje, es decir, cúantas veces el usuario ingresa un origen y un destino, hasta que decida terminar el programa.

Bucle Principal (while (cin >> origenViaje >> destinoViaje)): Este bucle se ejecuta S veces. Al hacer una solicitud, se ejecuta lo siguiente:

Bucle para encontrar el conductor más cercano (for (int i = 0; i < nConductores; i++)): Podemos notar que este bucle se ejecutará C veces, una vez por cada conductor en el sistema. Dentro de este bucle, la operación más costosa es la llamada a ciudad->BFS(...).


Análisis del algoritmo tGrafo::BFS

Inicialización de arreglos (distancia, predecesor, visitado): Estos arreglos son de tamaño cantidadNodos + 1. Su inicialización con el bucle for(int i = 0; i <= cantidadNodos; i++) toma O(V) tiempo.

Creación de la cola (tColaBFS cola()): El constructor de la cola asigna memoria para cantidadNodos + 1 elementos, lo que es O(V).

Bucle principal de BFS (while (!cola.isEmpty())): Cada nodo se encola y desencola a lo sumo una vez (V operaciones). Por cada nodo, se itera a través de sus aristas adyacentes. Utilizando una representación de grafo con listas de adyacencia, el costo total de iterar por todos los vecinos de todos los nodos es proporcional al número total de aristas en el grafo. Por lo tanto, esta parte del BFS toma O(V+E) tiempo.

Reconstrucción del camino: El bucle for (int i = d; i >= 0; i--) se ejecuta d+1 veces, donde d es la distancia del camino. En el peor caso, d puede ser hasta V−1. Por lo tanto, es O(V).

Liberación de memoria para arreglos internos de BFS: Los delete[] para distancia, predecesor, y visitado toman O(V) tiempo.

Conclusión: Una sola llamada a tGrafo::BFS tiene una complejidad temporal de O(V+E).

Volviendo al bucle de conductores: Dado que se realiza una llamada a BFS (O(V+E)) por cada uno de los C conductores, la complejidad de este bloque es O(C⋅(V+E)).

Recalcular la ruta del conductor al pasajero: Después de seleccionar el mejor conductor, se realiza otra llamada a ciudad->BFS() para obtener la ruta exacta. Esto es O(V+E).

Calcular la ruta y distancia del viaje del pasajero: Se realiza una tercera llamada a ciudad->BFS(...) para encontrar el camino del origen del pasajero a su destino. Esto es O(V+E).

Imprimir la ruta completa: El bucle para imprimir la ruta (for (int i = 0; i < largoRutaPasajeroDestino; i++)) puede recorrer hasta V nodos. Esto toma O(V) tiempo.

Liberar rutas dinámicas: Los delete[] para rutaConductorPasajero y rutaPasajeroDestino son O(V) en el peor caso, ya que el tamaño del camino puede ser V.

Complejidad de una sola iteración del bucle principal (while): 
Sumando las complejidades dentro de una iteración, tenemos:

O(C⋅(V+E)) + O(V+E) + O(V+E) + O(V) + O(V)

El término O(C⋅(V+E)) domina en crecimiento sobre los demás, por lo que una única iteración del bucle principal tiene una complejidad de O(C⋅(V+E)).



3. Fase de Liberación de Memoria Final
Esta fase ocurre al finalizar el programa.

Función liberarMemoria:

Liberar el grafo (delete ciudad): Esto invoca al destructor de tGrafo, el cual itera sobre todos los nodos y sus listas de adyacencia para liberar la memoria de las aristas. Esto toma O(V+E) tiempo.

Liberar los conductores: El bucle for para liberar las instancias Conductor se ejecuta C veces. Esto toma O(C) tiempo.

Complejidad total de liberarMemoria: O(V+E+C), o solo O(V+E).

Combinando todas las complejidades, sabemos que el tiempo total va a estar dado por la suma de las etapas antes descritas, entonces:

Tiempo total= O(V+E) + S⋅O(C⋅(V+E)) + O(V+E)

El término que domina el crecimiento del tiempo de ejecución cuando las entradas son grandes es el que depende de S y C⋅(V+E) (el tamaño del grafo por cada BFS).

Por lo tanto, la complejidad final de la función correrUber es O(S⋅C⋅(V+E)).


Impacto de la Implementación del TDA Grafo

La elección de la representación del grafo tiene un impacto directo en la complejidad de los algoritmos que operan sobre él. En este caso, la implementación de tGrafo utilizada es una lista de adyacencia (AristaAdyacencia** adyacencias). Esta es una decisión crucial ya que permite que el algoritmo BFS sea O(V+E). 

De haber optado por una matriz de adyacencia , una operación BFS tendría una complejidad de O(V^2), ya que para encontrar los vecinos de un nodo, el algoritmo tendría que revisar toda una fila (o columna) de la matriz, volviendo el tiempo de ejecución del programa muchísimo menos eficiente. 