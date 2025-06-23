// Código tarea 3 EDD

#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

//*********************************
//    Implementación del tGrafo 
//*********************************

// Nodos para construir el grafo dirigido
struct AristaAdyacencia {
    int destinoID;
    AristaAdyacencia* siguiente;

    //Constructor
    AristaAdyacencia(int destino) {
        destinoID = destino;
        siguiente = nullptr;
    }
};

// Clase grafo
class tGrafo {
private:
    int cantidadNodos;
    AristaAdyacencia** adyacencias;

public:
    // Constructor
    tGrafo(int nNodos) {
        cantidadNodos = nNodos;
        adyacencias = new AristaAdyacencia*[cantidadNodos + 1];
        for (int i = 0; i <= cantidadNodos; i++) {
            adyacencias[i] = nullptr;
        }
    }

    // Destructor para liberar memoria
    ~tGrafo() {
        for (int i = 0; i <= cantidadNodos; i++) {
            AristaAdyacencia* actual = adyacencias[i];
            while (actual != nullptr) {
                AristaAdyacencia* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }
        delete[] adyacencias;
        adyacencias = nullptr;
    }

    // Métodos
    // Agrega una arista dirigida desde el origenID al destinoID
    void setEdge(int origenID, int destinoID) {
        AristaAdyacencia* nuevaArista = new AristaAdyacencia(destinoID);
        nuevaArista->siguiente = adyacencias[origenID];
        adyacencias[origenID] = nuevaArista;
    }

    // Retorna el número de nodos
    int nVertex() {
        return cantidadNodos;
    }

    int BFS(int inicioID, int destinoID, int*& path, int& pathLength);

    // (Temporal) Para ver que todo se carga de forma correcta
    void imprimirGrafo() const {
        cout << "\n--- Estructura del Grafo ---" << endl;
        cout << "Número total de nodos: " << cantidadNodos << endl;

        for (int i = 1; i <= cantidadNodos; ++i) { // Los nodos van de 1 a numNodos
            cout << "Nodo " << i << ": ";
            AristaAdyacencia* actual = adyacencias[i];
            if (actual == nullptr) {
                cout << "No tiene aristas salientes.";
            } else {
                cout << "Adyacentes [";
                while (actual != nullptr) {
                    cout << actual->destinoID;
                    actual = actual->siguiente;
                    if (actual != nullptr) {
                        cout << ", ";
                    }
                }
                cout << "]";
            }
            cout << endl;
        }
        cout << "---------------------------\n" << endl;
    }
};

//**********************************
//      Implementación del BFS      
//**********************************

// Cola para BFS
class tColaBFS {
private:
    int* elementos;
    int capacidad;
    int frente;
    int final;
    int tamaño;

public:
    // Constructor
    tColaBFS(int maxNodos) {
        capacidad = maxNodos;
        frente = 0;
        final = -1;
        tamaño = 0;
        elementos = new int[capacidad];
    }

    // Destructor
    ~tColaBFS() {
        delete[] elementos;
    }

    // Métodos
    // Verifica si tiene elementos
    bool isEmpty() {
        return (tamaño == 0);
    }

    void enqueue(int valor) {
        if (tamaño < capacidad) {
            final = (final + 1) % capacidad;
            elementos[final] = valor;
            tamaño++;
        }
    }

    int dequeue() {
        if (!isEmpty()) {
            int valor = elementos[frente];
            frente = (frente + 1) % capacidad;
            tamaño--;
            return valor;
        }
        return -1;
    }
};

int tGrafo::BFS(int inicioID, int destinoID, int*& camino, int& largoCamino) {
    camino = nullptr;
    largoCamino = 0;

    // Para caminos negativos y nodos no existentes
    if (inicioID < 1 || inicioID > cantidadNodos || destinoID < 1 || destinoID > cantidadNodos) {
        return -1;
    }

    // Para caminos sin recorrido
    if (inicioID == destinoID) {
        largoCamino = 1;
        camino = new int[1];
        camino[0] = inicioID;
        return 0;
    }

    // Arreglos para la implementación de BFS
    int* distancia = new int[cantidadNodos + 1];
    int* predecesor = new int[cantidadNodos + 1];
    int* visitado = new int[cantidadNodos + 1];

    // Inicializar los arreglos
    for (int i = 0; i <= cantidadNodos; i++) {
        distancia[i] = -1;
        predecesor[i] = -1;
        visitado[i] = false;
    }

    // Crear la cola para BFS
    tColaBFS cola(cantidadNodos + 1);

    // Iniciar BFS desde el nodo de inicio
    cola.enqueue(inicioID);
    visitado[inicioID] = true;
    distancia[inicioID] = 0;

    int nodoActual;
    while (!cola.isEmpty()) {
        nodoActual = cola.dequeue();

        // Si llegamos al destino
        if (nodoActual == destinoID) {
            int d = distancia[destinoID];
            largoCamino = d + 1;
            camino = new int[largoCamino];

            // Reconstruimos el camino
            int temp = destinoID;
            for (int i = d; i >= 0; i--) {
                camino[i] = temp;
                temp = predecesor[temp];
            }

            delete[] distancia;
            delete[] predecesor;
            delete[] visitado;

            return d;
        }

        // Recorrer los vecinos del nodo actual
        AristaAdyacencia* adyacente = adyacencias[nodoActual];
        while (adyacente != nullptr) {
            int vecinoID = adyacente->destinoID;
            if (!visitado[vecinoID]) {
                visitado[vecinoID] = true;
                distancia[vecinoID] = distancia[nodoActual] + 1;
                predecesor[vecinoID] = nodoActual;
                cola.enqueue(vecinoID);
            }
            adyacente = adyacente->siguiente;
        }
    }

    // Si la cola esta vacía no se encontró el destino, no hay camino posible
    delete[] distancia;
    delete[] predecesor;
    delete[] visitado;

    return -1;
}


//************************************
//    Implementación del Conductor 
//************************************

// Clase para los conductores
class Conductor {
private:
    int conductorID;
    int posicionActual; // ID del nodo

public:
    // Constructor
    Conductor(int idConductor, int posicion) {
        conductorID = idConductor;
        posicionActual = posicion;
    }

    // Métodos
    // Obtener la posición actual del conductor
    int getPosicion () {
        return posicionActual;
    }

    // Modificar la posición actual del conductor
    void setPosicion(int nuevoNodoID) {
        posicionActual = nuevoNodoID;
    }

    // Obtener el ID del conductor
    int getConductorID() {
        return conductorID;
    }
};

//*************************************
//    Implementación del la lectura
//*************************************

// Función para leer el archivo .txt
bool leerArchivo(string nombreArchivo, tGrafo*& grafo, Conductor**& conductores, int& nConductores) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo" << endl;
        return false;
    }
    int nNodos, nArcos;
    archivo >> nNodos >> nArcos >> nConductores;

    grafo = new tGrafo(nNodos);

    for (int i = 0; i < nArcos; i++) {
        int origen, destino;
        archivo >> origen >> destino;
        grafo->setEdge(origen, destino);
    }

    conductores = new Conductor*[nConductores];
    for (int i = 0; i < nConductores; i++) {
        int nodoConductor;
        archivo >> nodoConductor;
        conductores[i] = new Conductor(i + 1, nodoConductor);
    }

    archivo.close();
    return true;
} 

//*************************
//     Liberar Memoria
//*************************

// Función para liberar la memoria dinamica al finalizar el programa
void liberarMemoria(tGrafo*& grafo, Conductor**& conductores, int& nConductores) {
    if (grafo != nullptr) {
        delete grafo;
        grafo = nullptr;
    }
    if (conductores != nullptr) {
        for (int i = 0; i < nConductores; ++i) {
            if (conductores[i] != nullptr) {
                delete conductores[i];
                conductores[i] = nullptr;
            }
        }
        delete[] conductores;
        conductores = nullptr;
    }
    nConductores = 0;
}

//************************************
//               Uber 
//************************************

// Función para utilizar Uber
void solicitar_uber(string nombreArchivo) {
    tGrafo* ciudad = nullptr;
    Conductor** conductores = nullptr;
    int nConductores = 0;
    if (!leerArchivo(nombreArchivo, ciudad, conductores, nConductores)) {
        return;
    }

    // (Temporal) Para ver que todo se carga de forma correcta
    ciudad->imprimirGrafo(); 
    cout << "--- Conductores Cargados ---" << endl;
    cout << "Número de Conductores = " << nConductores << endl;
    for (int i = 0; i < nConductores; i++) {
        cout << "Conductor " << conductores[i]->getConductorID() << " en nodo: " << conductores[i]->getPosicion() << endl;
    }
    cout << "---------------------------\n" << endl;

    // Procesar las solicitudes de viaje
    cout << "Ingrese viaje: ";
    int origenViaje, destinoViaje;
    while (cin >> origenViaje >> destinoViaje) {
        if (origenViaje == -1 && destinoViaje == -1) break; // Terminar el programa

        // Variables para las rutas y costos
        int mejorCosto = -1;
        int distanciaMinConductor = -1;
        int distanciaPasajero = -1;
        int conductorSeleccionadoID = -1;

        int* rutaConductorPasajero = nullptr;
        int largoRutaConductorPasajero = 0;
        int * rutaPasajeroDestino = nullptr;
        int largoRutaPasajeroDestino = 0;

        // Encontrar el conductor más cercano al pasajero
        for (int i = 0; i < nConductores; i++) {
            int* caminoTemp = nullptr;
            int largoTemp = 0;
            int distanciaConductorActual = ciudad->BFS(conductores[i]->getPosicion(), origenViaje, caminoTemp, largoTemp);
            
            // Liberar memoria del camino de BFS si se asignó
            if (caminoTemp != nullptr) {
                delete[] caminoTemp;
                caminoTemp = nullptr;
            }

            // Si hay una ruta desde este conductor
            if (distanciaConductorActual != -1) {
                if (distanciaMinConductor == -1 || distanciaConductorActual < distanciaMinConductor) {
                    distanciaMinConductor = distanciaConductorActual;
                    conductorSeleccionadoID = i;
                }
                else if (distanciaConductorActual == distanciaMinConductor) {
                    // Criterio de desempate, el conductor ubicado en el nodo de índice menor 
                    if (conductores[i]->getPosicion() < conductores[conductorSeleccionadoID]->getPosicion()) {
                        conductorSeleccionadoID = i;
                    }
                }
            }
        }

        // Si se encuentra conductor, se calcula el viaje y el costo
        if (conductorSeleccionadoID != -1) {
            // Recalcular la ruta del conductor al pasajero para mostrarla
            ciudad->BFS(conductores[conductorSeleccionadoID]->getPosicion(), origenViaje, rutaConductorPasajero, largoRutaConductorPasajero);

            // Calcular la ruta y distancia del viaje del pasajero 
            distanciaPasajero = ciudad->BFS(origenViaje, destinoViaje, rutaPasajeroDestino, largoRutaPasajeroDestino);
            
            // Si hay una ruta para el viaje del pasajero
            if (distanciaPasajero != -1) {
                // Calcular el costo total 
                mejorCosto = 300 * distanciaMinConductor + 500 * distanciaPasajero;

                // Imprimir la ruta completa
                cout << "Ruta: ";
                if (rutaPasajeroDestino != nullptr && largoRutaPasajeroDestino > 0) {
                    cout << "{";
                    for (int i = 0; i < largoRutaPasajeroDestino; i++) {
                        cout << rutaPasajeroDestino[i];
                        if (i < largoRutaPasajeroDestino - 1) {
                            cout << ", ";
                        }
                    }
                    cout << "}";
                }
                cout << " - Costo: " << mejorCosto << endl;

                // Actualizar la nueva posición del conductor
                conductores[conductorSeleccionadoID]->setPosicion(destinoViaje);
            }
            // Viaje imposible
            else {
                cout << "Ruta: {} - Costo: -1" << endl;
            }
        }
        // No se encontró ningún conductor que pueda llegar al origen del pasajero 
        else {
            cout << "Ruta: {} - Costo: -1" << endl;
        }

        // Liberar las rutas dinámicas después de cada viaje
        if (rutaPasajeroDestino != nullptr) {
            delete[] rutaPasajeroDestino;
            rutaPasajeroDestino = nullptr;
        }
        if (rutaConductorPasajero != nullptr) {
            delete[] rutaConductorPasajero;
            rutaConductorPasajero = nullptr;
        }

        // Limpiar el buffer de entrada para el siguiente input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ingrese viaje: ";
    }

    // Liberar toda la memoria dinámica antes de terminar el programa
    liberarMemoria(ciudad, conductores, nConductores);

    return;
}

//************************************
//               Main 
//************************************

// Main de la aplicación Uber
int main() {
    // Para modificar el archivo desde donde se carga el mapa
    string nombreArchivo = "data2.txt";

    solicitar_uber(nombreArchivo);

    return 0;
}