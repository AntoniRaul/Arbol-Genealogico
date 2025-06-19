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
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), izquierda(nullptr), derecha(nullptr) {}
};

// Función para agregar un nuevo miembro al árbol genealógico

void agregarMiembro(Nodo*& raiz) {
    string nombre, genero, relacion;
    cout << "Ingrese el nombre completo: ";
    cin.ignore(); // Limpia el búfer para evitar problemas con getline
    getline(cin, nombre);
    cout << "Ingrese el género: ";
    getline(cin, genero);
    cout << "Ingrese la relación familiar: ";
    getline(cin, relacion);

    Nodo* nuevo = new Nodo(nombre, genero, relacion);

    // Si el árbol está vacío, el nuevo nodo será la raíz
    if (raiz == nullptr) {
        raiz = nuevo;
        cout << "Miembro agregado como raíz del árbol." << endl;
        return;
    }

    // Busca la posición correcta para insertar el nuevo nodo
    Nodo* actual = raiz;
    Nodo* padre = nullptr;
    while (actual != nullptr) {
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
    if (raiz == nullptr) {
        return false; // No se encontró el miembro
    }

    if (nombre < raiz->nombreCompleto) {
        return eliminarMiembro(raiz->izquierda, nombre);
    } else if (nombre > raiz->nombreCompleto) {
        return eliminarMiembro(raiz->derecha, nombre);
    } else {
        // Nodo encontrado
        if (raiz->izquierda == nullptr && raiz->derecha == nullptr) {
            // Sin hijos
            cout << "El miembro \"" << raiz->nombreCompleto << "\" no tiene descendencia. Eliminando..." << endl;
            delete raiz;
            raiz = nullptr;
        } else if (raiz->izquierda == nullptr || raiz->derecha == nullptr) {
            // Un solo hijo
            cout << "ALERTA: El miembro \"" << raiz->nombreCompleto << "\" tiene descendencia. Eliminando y reubicando descendientes..." << endl;
            Nodo* temp = (raiz->izquierda != nullptr) ? raiz->izquierda : raiz->derecha;
            delete raiz;
            raiz = temp;
        } else {
            // Dos hijos
            cout << "ALERTA: El miembro \"" << raiz->nombreCompleto << "\" tiene descendencia. Eliminando y reubicando descendientes..." << endl;
            // Buscar el sucesor inorden (el menor de la rama derecha)
            Nodo* sucesor = raiz->derecha;
            Nodo* padreSucesor = raiz;
            while (sucesor->izquierda != nullptr) {
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
        cout << "Miembro no encontrado en el árbol." << endl;
    }
}


void mostrarMenu() {
    cout << "===== MENU ARBOL GENEALOGICO =====" << endl;
    cout << "1. Agregar miembro al árbol genealógico" << endl;
    cout << "2. Eliminar miembro del árbol genealógico" << endl;
    cout << "3. Buscar miembro" << endl;
    cout << "4. Visualizar el árbol (recorridos)" << endl;
    cout << "5. Mostrar ancestros" << endl;
    cout << "6. Mostrar descendientes" << endl;
    cout << "7. Verificar pertenencia a una rama" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opción: ";
}

// Función recursiva para guardar el árbol en un archivo

void guardarArbol(Nodo* raiz, ofstream& archivo) {
    if (raiz == nullptr) return;
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
    cout << "Árbol guardado correctamente en 'arbol.txt'." << endl;
}

// Función para insertar un nuevo nodo en el árbol genealógico
void insertarNodo(Nodo*& raiz, const string& nombre, const string& genero, const string& relacion) {
    Nodo* nuevo = new Nodo(nombre, genero, relacion);
    if (raiz == nullptr) {
        raiz = nuevo;
        return;
    }
    Nodo* actual = raiz;
    Nodo* padre = nullptr;
    while (actual != nullptr) {
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

int main() {
    Nodo* raiz = nullptr;
    int opcion;

    // Cargar datos al iniciar
    cargarArbolDesdeArchivo(raiz);

    do {
        mostrarMenu();
        cin >> opcion;
        switch(opcion) {
            case 1:
                agregarMiembro(raiz);
                break;
            case 2:
                eliminarMiembroPrompt(raiz);
                break;
            case 3:
                cout << "Buscar miembro" << endl;
                break;
            case 4:
                cout << "Visualizar árbol" << endl;
                break;
            case 5:
                cout << "Mostrar ancestros" << endl;
                break;
            case 6:
                cout << "Mostrar descendientes" << endl;
                break;
            case 7:
                cout << "Verificar pertenencia a rama" << endl;
                break;
            case 0:
                // Guardar datos antes de salir
                guardarArbolEnArchivo(raiz);
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }
        cout << endl;
    } while(opcion != 0);

    return 0;
}
