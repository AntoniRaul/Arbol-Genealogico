#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Estructura para un nodo del arbol genealogico
struct Nodo {
    string nombreCompleto;      // Nombre completo del miembro
    string genero;              // Genero del miembro
    string relacionFamiliar;    // Relacion familiar
    Nodo* izquierda;            // Puntero al hijo izquierdo
    Nodo* derecha;              // Puntero al hijo derecho

    // Constructor para inicializar los datos del nodo
    Nodo(string nombre, string gen, string relacion)
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), izquierda(NULL), derecha(NULL) {}
};

// Funcion para agregar un nuevo miembro al arbol genealogico

void agregarMiembro(Nodo*& raiz) {
    string nombre, genero, relacion;
    cout << "Ingrese el nombre completo: ";
    cin >> nombre; // Usa cin para leer el nombre
    cout << "Ingrese el genero: ";
    cin >> genero; // Usa cin para leer el genero
    cout << "Ingrese la relacion familiar: ";
    cin >> relacion; // Usa cin para leer la relacion

    Nodo* nuevo = new Nodo(nombre, genero, relacion);

    // Si el arbol esta vacio, el nuevo nodo sera la raiz
    if (raiz == NULL) {
        raiz = nuevo;
        cout << "Miembro agregado como raiz del arbol." << endl;
        return;
    }

    // Busca la posicion correcta para insertar el nuevo nodo
    Nodo* actual = raiz;
    Nodo* padre = NULL;
    while (actual != NULL) {
        padre = actual;
        if (nombre < actual->nombreCompleto)
            actual = actual->izquierda;
        else
            actual = actual->derecha;
    }
    // Inserta el nuevo nodo como hijo izquierdo o derecho segun corresponda
    if (nombre < padre->nombreCompleto)
        padre->izquierda = nuevo;
    else
        padre->derecha = nuevo;

    cout << "Miembro agregado correctamente." << endl;
}

// Funcion recursiva para eliminar un miembro del arbol genealagico

bool eliminarMiembro(Nodo*& raiz, const string& nombre) {
    if (raiz == NULL) {
        return false; // No se encontro el miembro
    }

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

// Función para buscar un miembro en el árbol
Nodo* buscarMiembro(Nodo* raiz, const string& nombre) {
    if (raiz == NULL || raiz->nombreCompleto == nombre) {
        return raiz;
    }
    if (nombre < raiz->nombreCompleto) {
        return buscarMiembro(raiz->izquierda, nombre);
    } else {
        return buscarMiembro(raiz->derecha, nombre);
    }
}

// Función para mostrar los datos de un miembro encontrado
void buscarMiembroPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a buscar: ";
    cin >> nombre; // Usa cin para leer el nombre directamente

    Nodo* encontrado = buscarMiembro(raiz, nombre);
    if (encontrado != NULL) {
        cout << "\n=== Miembro Encontrado ===" << endl;
        cout << "Nombre: " << encontrado->nombreCompleto << endl;
        cout << "Género: " << encontrado->genero << endl;
        cout << "Relación: " << encontrado->relacionFamiliar << endl;
    } else {
        cout << "Miembro no encontrado en el árbol." << endl;
    }
}

// Función para mostrar los ancestros de un miembro (camino desde la raíz hasta el nodo)
void mostrarAncestros(Nodo* raiz, const string& nombre) {
    if (raiz == NULL) {
        cout << "Miembro no encontrado." << endl;
        return;
    }
    if (raiz->nombreCompleto == nombre) {
        cout << "=== Ancestros de " << nombre << " ===" << endl;
        return;
    }
    cout << raiz->nombreCompleto << " -> ";
    if (nombre < raiz->nombreCompleto) {
        mostrarAncestros(raiz->izquierda, nombre);
    } else {
        mostrarAncestros(raiz->derecha, nombre);
    }
}

// Función para solicitar el nombre del miembro y mostrar sus ancestros
void mostrarAncestrosPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro para ver sus ancestros: ";
    cin >> nombre; // Usa cin para leer el nombre directamente

    if (buscarMiembro(raiz, nombre) == NULL) {
        cout << "Miembro no encontrado." << endl;
        return;
    }
    mostrarAncestros(raiz, nombre);
}

// Función auxiliar para recorrer y mostrar todos los descendientes de un nodo (inorden)
void mostrarDescendientesRec(Nodo* nodo) {
    if (nodo == NULL) return;
    mostrarDescendientesRec(nodo->izquierda);
    cout << "- " << nodo->nombreCompleto << " (" << nodo->relacionFamiliar << ")" << endl;
    mostrarDescendientesRec(nodo->derecha);
}

// Función para mostrar los descendientes de un miembro
void mostrarDescendientes(Nodo* raiz, const string& nombre) {
    Nodo* miembro = buscarMiembro(raiz, nombre);
    if (miembro == NULL) {
        cout << "Miembro no encontrado." << endl;
        return;
    }
    cout << "\n=== Descendientes de " << nombre << " ===" << endl;
    if (miembro->izquierda == NULL && miembro->derecha == NULL) {
        cout << "No tiene descendientes." << endl;
    } else {
        mostrarDescendientesRec(miembro->izquierda);
        mostrarDescendientesRec(miembro->derecha);
    }
}

// Función para solicitar el nombre del miembro y mostrar sus descendientes
void mostrarDescendientesPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro para ver sus descendientes: ";
    cin >> nombre; // Usa cin para leer el nombre directamente

    mostrarDescendientes(raiz, nombre);
}


// Funcion para solicitar al usuario el nombre del miembro a eliminar y llamar a la funcion de eliminacion
void eliminarMiembroPrompt(Nodo*& raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a eliminar: ";
    cin >> nombre; // Usa cin para leer el nombre directamente

    if (!eliminarMiembro(raiz, nombre)) {
        cout << "Miembro no encontrado en el arbol." << endl;
    }
}

void visualizarArbol(Nodo*& raiz) {
	
}

void mostrarMenu() {
    cout << "===== MENU ARBOL GENEALOGICO =====" << endl;
    cout << "1. Agregar miembro al arbol genealogico" << endl;
    cout << "2. Eliminar miembro del arbol genealogico" << endl;
    cout << "3. Buscar miembro" << endl;
    cout << "4. Visualizar el arbol (recorridos)" << endl;
    cout << "5. Mostrar ancestros" << endl;
    cout << "6. Mostrar descendientes" << endl;
    cout << "7. Verificar pertenencia a una rama" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opcian: ";
}

// Funcion recursiva para guardar el arbol en un archivo

void guardarArbol(Nodo* raiz, ofstream& archivo) {
    if (raiz == NULL) return;
    guardarArbol(raiz->izquierda, archivo);
    archivo << raiz->nombreCompleto << "|" << raiz->genero << "|" << raiz->relacionFamiliar << endl;
    guardarArbol(raiz->derecha, archivo);
}

// Funcion para guardar el arbol genealogico en un archivo de texto

void guardarArbolEnArchivo(Nodo* raiz) {
    ofstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para guardar." << endl;
        return;
    }
    guardarArbol(raiz, archivo);
    archivo.close();
    cout << "arbol guardado correctamente en 'arbol.txt'." << endl;
}

// Funcion para insertar un nuevo nodo en el arbol genealogico
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

// Funcion para cargar el arbol genealogico desde un archivo de texto
void cargarArbolDesdeArchivo(Nodo*& raiz) {
    ifstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para cargar." << endl;
        return;
    }

    string nombre, genero, relacion;
    while (archivo >> nombre >> genero >> relacion) { // Leer directamente usando >>
        insertarNodo(raiz, nombre, genero, relacion);
    }

    archivo.close();
    cout << "Arbol cargado correctamente desde 'arbol.txt'." << endl;
}

int main() {
    Nodo* raiz = NULL;
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
                cout << "Visualizar arbol" << endl;
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
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
        cout << endl;
    } while(opcion != 0);

    return 0;
}
