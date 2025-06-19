#include <iostream>
#include <string>
using namespace std;

// Estructura para un nodo del árbol genealógico
struct Nodo {
    string nombreCompleto;
    string genero;
    string relacionFamiliar;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(string nombre, string gen, string relacion)
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), izquierda(nullptr), derecha(nullptr) {}
};

// Función para agregar un miembro al árbol genealógico (ABB por nombre)
void agregarMiembro(Nodo*& raiz) {
    string nombre, genero, relacion;
    cout << "Ingrese el nombre completo: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese el género: ";
    getline(cin, genero);
    cout << "Ingrese la relación familiar: ";
    getline(cin, relacion);

    Nodo* nuevo = new Nodo(nombre, genero, relacion);

    if (raiz == nullptr) {
        raiz = nuevo;
        cout << "Miembro agregado como raíz del árbol." << endl;
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

    cout << "Miembro agregado correctamente." << endl;
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

int main() {
    Nodo* raiz = nullptr;
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        switch(opcion) {
            case 1:
                agregarMiembro(raiz);
                break;
            case 2:
                cout << "Eliminar miembro" << endl;
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
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }
        cout << endl;
    } while(opcion != 0);

    return 0;
}
