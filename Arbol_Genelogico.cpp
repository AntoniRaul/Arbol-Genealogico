#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Estructura para un nodo del árbol genealógico
struct Nodo {
    string nombreCompleto;      // Nombre completo del miembro
    string genero;              // Género del miembro
    string relacionFamiliar;    // Relación familiar (ej: padre, madre, hijo)
    Nodo* izquierda;            // Puntero al hijo izquierdo (menor alfabéticamente)
    Nodo* derecha;              // Puntero al hijo derecho (mayor alfabéticamente)

    // Constructor para inicializar los datos del nodo
    Nodo(string nombre, string gen, string relacion)
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), izquierda(NULL), derecha(NULL) {}
};

// Función para agregar un nuevo miembro al árbol genealógico

void agregarMiembro(Nodo*& raiz) {
    string nombre, genero, relacion;
    cout << "Ingrese el nombre completo: ";
    cin.ignore(); // Limpia el búfer para evitar problemas con getline
    getline(cin, nombre);
    cout << "Ingrese el genero: ";
    getline(cin, genero);
    cout << "Ingrese la relacion familiar: ";
    getline(cin, relacion);

    Nodo* nuevo = new Nodo(nombre, genero, relacion);

    // Si el árbol está vacío, el nuevo nodo será la raíz
    if (raiz == NULL) {
        raiz = nuevo;
        cout << "Miembro agregado como raíz del árbol." << endl;
        return;
    }

    // Busca la posición correcta para insertar el nuevo nodo
    Nodo* actual = raiz;
    Nodo* padre = NULL;
    while (actual != NULL) {
        padre = actual;
        if (nombre < actual->nombreCompleto)
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }
    // Inserta el nuevo nodo como hijo izquierdo o derecho según corresponda
    if (nombre < padre->nombreCompleto)
        padre->izquierda = nuevo;
    else
        padre->derecha = nuevo;

    cout << "Miembro agregado correctamente." << endl;
}

// Función recursiva para eliminar un miembro del árbol genealógico

bool eliminarMiembro(Nodo*& raiz, const string& nombre) {
    if (raiz == NULL) {
        return false; // No se encontró el miembro
    }
// Buscar por nombre en el subárbol izquierdo o derecho
    if (nombre < raiz->nombreCompleto) {
        return eliminarMiembro(raiz->izquierda, nombre);
    } else if (nombre > raiz->nombreCompleto) {
        return eliminarMiembro(raiz->derecha, nombre);
    } else {
        // Nodo encontrado
        if (raiz->izquierda == NULL && raiz->derecha == NULL) {
            // Sin hijos
            cout << "El miembro \"" << raiz->nombreCompleto << "\" no tiene descendencia. Eliminando..." << endl;
            delete raiz;
            raiz = NULL;
        } else if (raiz->izquierda == NULL || raiz->derecha == NULL) {
            // Un solo hijo
            cout << "ALERTA: El miembro \"" << raiz->nombreCompleto << "\" tiene descendencia. Eliminando y reubicando descendientes..." << endl;
            Nodo* temp = (raiz->izquierda != NULL) ? raiz->izquierda : raiz->derecha;
            delete raiz;
            raiz = temp;
        } else {
            // Dos hijos
            cout << "ALERTA: El miembro \"" << raiz->nombreCompleto << "\" tiene descendencia. Eliminando y reubicando descendientes..." << endl;
            // Buscar el sucesor inorden (el menor de la rama derecha)
            Nodo* sucesor = raiz->derecha;
            Nodo* padreSucesor = raiz;
            while (sucesor->izquierda != NULL) {
                padreSucesor = sucesor;
                sucesor = sucesor->izquierda;
            }
            // Copiar datos del sucesor
            raiz->nombreCompleto = sucesor->nombreCompleto;
            raiz->genero = sucesor->genero;
            raiz->relacionFamiliar = sucesor->relacionFamiliar;
            // Eliminar el sucesor
            if (padreSucesor->izquierda == sucesor)
                eliminarMiembro(padreSucesor->izquierda, sucesor->nombreCompleto);
            else
                eliminarMiembro(padreSucesor->derecha, sucesor->nombreCompleto);
        }
        return true;
    }
}

// Función para solicitar al usuario el nombre del miembro a eliminar y llamar a la función de eliminación
void eliminarMiembroPrompt(Nodo*& raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a eliminar: ";
    cin.ignore();
    getline(cin, nombre);

    if (!eliminarMiembro(raiz, nombre)) {
        cout << "Miembro no encontrado en el arbol." << endl;
    }
}

void mostrarMenu() {
    cout << "===== MENU ARBOL GENEALOGICO =====" << "\n";
    cout << "1. Agregar miembro al arbol genealogico" << "\n";
    cout << "2. Eliminar miembro del arbol genealogico" << "\n";
    cout << "3. Buscar miembro" << "\n";
    cout << "4. Visualizar el arbol (recorridos)" << "\n";
    cout << "5. Mostrar ancestros" << "\n";
    cout << "6. Mostrar descendientes" << "\n";
    cout << "7. Verificar pertenencia a una rama" << "\n";
    cout << "0. Salir" << "\n";
    cout << "Seleccione una opcion: ";
}

// Función recursiva para guardar el árbol en un archivo

void guardarArbol(Nodo* raiz, ofstream& archivo) {
    if (raiz == NULL) return;
    guardarArbol(raiz->izquierda, archivo);
    archivo << raiz->nombreCompleto << "|" << raiz->genero << "|" << raiz->relacionFamiliar << endl;
    guardarArbol(raiz->derecha, archivo);
}

// Función para guardar el árbol genealógico en un archivo de texto

void guardarArbolEnArchivo(Nodo* raiz) {
    ofstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para guardar." << endl;
        return;
    }
    guardarArbol(raiz, archivo);
    archivo.close();
    cout << "Arbol guardado correctamente en 'arbol.txt'." << endl;
}

// Función para insertar un nuevo nodo en el árbol genealógico
void insertarNodo(Nodo*& raiz, const string& nombre, const string& genero, const string& relacion) {
    Nodo* nuevo = new Nodo(nombre, genero, relacion);
    if (raiz == NULL) {
        raiz = nuevo;
        return;
    }
    Nodo* actual = raiz;
    Nodo* padre = NULL;
    while (actual != NULL) {
        padre = actual;
        if (nombre < actual->nombreCompleto)
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }
    if (nombre < padre->nombreCompleto)
        padre->izquierda = nuevo;
    else
        padre->derecha = nuevo;
}

// Función para cargar el árbol genealógico desde un archivo de texto
void cargarArbolDesdeArchivo(Nodo*& raiz) {
    ifstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para cargar." << endl;
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string nombre, genero, relacion;
        getline(ss, nombre, '|');
        getline(ss, genero, '|');
        getline(ss, relacion, '|');
        insertarNodo(raiz, nombre, genero, relacion);
    }
    archivo.close();
    cout << "Árbol cargado correctamente desde 'arbol.txt'." << endl;
}
// Recorrido Inorden: izquierda -> raíz -> derecha
void recorridoInorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorridoInorden(raiz->izquierda);
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
    recorridoInorden(raiz->derecha);
}

// Recorrido Preorden: raíz -> izquierda -> derecha
void recorridoPreorden(Nodo* raiz) {
    if (raiz == NULL) return;
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
    recorridoPreorden(raiz->izquierda);
    recorridoPreorden(raiz->derecha);
}

// Recorrido Postorden: izquierda -> derecha -> raíz
void recorridoPostorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorridoPostorden(raiz->izquierda);
    recorridoPostorden(raiz->derecha);
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
}
// Función para mostrar opciones de recorrido del árbol
void visualizarArbol(Nodo* raiz) {
    int opcion;
    cout << "=== Visualización del Arbol Genealógico ===" << endl;
    cout << "1. Recorrido Inorden (ordenado alfabeticamente)" << endl;
    cout << "2. Recorrido Preorden (estructura de arbol)" << endl;
    cout << "3. Recorrido Postorden (descendientes primero)" << endl;
    cout << "Seleccione un tipo de recorrido: ";
    cin >> opcion;

    cout << "\nResultado del recorrido:" << endl;
    switch(opcion) {
        case 1:
            recorridoInorden(raiz);
            break;
        case 2:
            recorridoPreorden(raiz);
            break;
        case 3:
            recorridoPostorden(raiz);
            break;
        default:
            cout << "Opción inválida." << endl;
    }
}

int main() {
    Nodo* raiz = NULL; // Árbol vacío al inicio
    int opcion;

    // Cargar árbol previamente guardado si existe
    cargarArbolDesdeArchivo(raiz);

    // Menú interactivo principal
    do {
        mostrarMenu();
        cin >> opcion;
        switch(opcion) {
            case 1:
                agregarMiembro(raiz); // Agregar nuevo miembro
                break;
            case 2:
                eliminarMiembroPrompt(raiz); // Eliminar miembro existente
                break;
            case 3:
                cout << "Buscar miembro" << "\n"; 
                break;
            case 4:
                visualizarArbol(raiz);
                break;
            case 5:
                cout << "Mostrar ancestros" << "\n"; 
                break;
            case 6:
                cout << "Mostrar descendientes" << "\n"; 
                break;
            case 7:
                cout << "Verificar pertenencia a rama" << "\n"; 
                break;
            case 0:
                guardarArbolEnArchivo(raiz); // Guardar antes de salir
                cout << "Saliendo..." << "\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << "\n";
        }
        cout << endl;
    } while(opcion != 0); // Repetir mientras no se elija salir

    return 0;
}
