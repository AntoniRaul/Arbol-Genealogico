#include <iostream> // Incluye la biblioteca estándar para entrada y salida
#include <string>   // Incluye la biblioteca estándar para manejo de strings
using namespace std; // Usa el espacio de nombres estándar

struct Persona {
    string nombre; // Nombre de la persona
    Persona* padre; // Puntero al padre
    Persona* madre; // Puntero a la madre
    Persona* hijoIzquierdo; // Puntero al primer hijo
    Persona* hermanoDerecho; // Puntero al siguiente hermano

    Persona(string nombre) : nombre(nombre), padre(NULL), madre(NULL), hijoIzquierdo(NULL), hermanoDerecho(NULL) {}
};

void agregarPadres(Persona* hijo, Persona* padre, Persona* madre) {
    hijo->padre = padre; // Asigna el padre al hijo
    hijo->madre = madre; // Asigna la madre al hijo
}

void agregarHijo(Persona* padre, Persona* hijo) {
    if (!padre->hijoIzquierdo) {
        padre->hijoIzquierdo = hijo; // Si no tiene hijos, asigna el nuevo hijo como primer hijo
    } else {
        Persona* temp = padre->hijoIzquierdo;
        while (temp->hermanoDerecho) {
            temp = temp->hermanoDerecho; // Busca el último hermano
        }
        temp->hermanoDerecho = hijo; // Agrega el nuevo hijo como hermano derecho
    }
}

void imprimirArbol(Persona* persona, int nivel = 0) {
    if (!persona) return; // Si la persona es nula, termina la función
    for (int i = 0; i < nivel; ++i) cout << "  "; // Indenta según el nivel
    cout << persona->nombre << endl; // Imprime el nombre de la persona
    imprimirArbol(persona->hijoIzquierdo, nivel + 1); // Imprime los hijos
    imprimirArbol(persona->hermanoDerecho, nivel); // Imprime los hermanos
}

void crearPersonaRaiz(Persona*& raiz) {
    if (raiz) {
        cout << "Ya existe una persona raíz." << endl; // Si ya existe la raíz, muestra mensaje
    } else {
        string nombre;
        cout << "Nombre de la persona raíz: ";
        cin >> nombre; // Lee el nombre de la raíz
        raiz = new Persona(nombre); // Crea la persona raíz
    }
}

void opcionAgregarHijo(Persona* raiz) {
    if (!raiz) {
        cout << "Primero cree la persona raíz." << endl; // Si no hay raíz, muestra mensaje
        return;
    }
    string nombrePadre, nombreHijo;
    cout << "Nombre del padre/madre existente: ";
    cin >> nombrePadre; // Lee el nombre del padre/madre
    cout << "Nombre del nuevo hijo: ";
    cin >> nombreHijo; // Lee el nombre del hijo

    Persona* stack[100];
    int top = 0;
    stack[top++] = raiz;
    Persona* padre = nullptr;
    while (top > 0) {
        Persona* actual = stack[--top];
        if (actual->nombre == nombrePadre) {
            padre = actual; // Encuentra el padre/madre
            break;
        }
        if (actual->hijoIzquierdo) stack[top++] = actual->hijoIzquierdo;
        Persona* hermano = actual->hijoIzquierdo ? actual->hijoIzquierdo->hermanoDerecho : nullptr;
        while (hermano) {
            stack[top++] = hermano;
            hermano = hermano->hermanoDerecho;
        }
    }
    if (!padre) {
        cout << "No se encontró al padre/madre." << endl; // Si no se encuentra, muestra mensaje
    } else {
        Persona* hijo = new Persona(nombreHijo); // Crea el nuevo hijo
        agregarHijo(padre, hijo); // Agrega el hijo al padre
        cout << "Hijo agregado." << endl;
    }
}

void opcionAgregarPadres(Persona* raiz) {
    if (!raiz) {
        cout << "Primero cree la persona raíz." << endl; // Si no hay raíz, muestra mensaje
        return;
    }
    string nombreHijo, nombrePadre, nombreMadre;
    cout << "Nombre del hijo: ";
    cin >> nombreHijo; // Lee el nombre del hijo
    cout << "Nombre del padre: ";
    cin >> nombrePadre; // Lee el nombre del padre
    cout << "Nombre de la madre: ";
    cin >> nombreMadre; // Lee el nombre de la madre

    Persona* stack[100];
    int top = 0;
    stack[top++] = raiz;
    Persona* hijo = nullptr;
    while (top > 0) {
        Persona* actual = stack[--top];
        if (actual->nombre == nombreHijo) {
            hijo = actual; // Encuentra al hijo
            break;
        }
        if (actual->hijoIzquierdo) stack[top++] = actual->hijoIzquierdo;
        Persona* hermano = actual->hijoIzquierdo ? actual->hijoIzquierdo->hermanoDerecho : nullptr;
        while (hermano) {
            stack[top++] = hermano;
            hermano = hermano->hermanoDerecho;
        }
    }
    if (!hijo) {
        cout << "No se encontró al hijo." << endl; // Si no se encuentra, muestra mensaje
    } else {
        Persona* padre = new Persona(nombrePadre); // Crea el padre
        Persona* madre = new Persona(nombreMadre); // Crea la madre
        agregarPadres(hijo, padre, madre); // Asigna los padres al hijo
        cout << "Padres agregados." << endl;
    }
}

void opcionImprimirArbol(Persona* raiz) {
    imprimirArbol(raiz); // Llama a la función para imprimir el árbol
}

void mostrarMenu() {
    cout << "1. Crear persona raíz" << endl;
    cout << "2. Agregar hijo" << endl;
    cout << "3. Agregar padres a una persona" << endl;
    cout << "4. Imprimir árbol" << endl;
    cout << "0. Salir" << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    Persona* raiz = nullptr; // Inicializa la raíz como nula
    int opcion;
    do {
        mostrarMenu(); // Muestra el menú de opciones
        cin >> opcion; // Lee la opción del usuario
        switch (opcion) {
            case 1:
                crearPersonaRaiz(raiz); // Crea la persona raíz
                break;
            case 2:
                opcionAgregarHijo(raiz); // Agrega un hijo
                break;
            case 3:
                opcionAgregarPadres(raiz); // Agrega padres a una persona
                break;
            case 4:
                opcionImprimirArbol(raiz); // Imprime el árbol
                break;
            case 0:
                cout << "Saliendo..." << endl; // Mensaje de salida
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl; // Mensaje de opción inválida
        }
        cout << endl; // Salto de línea para separar las operaciones
    } while (opcion != 0);

    return 0; // Fin del programa
}
