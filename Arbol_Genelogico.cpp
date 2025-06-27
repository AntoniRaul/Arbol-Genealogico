#include <iostream> // Incluye la biblioteca estándar para entrada y salida.
#include <string>   // Incluye la biblioteca para manejar cadenas de texto.
#include <fstream>  // Incluye la biblioteca para manejo de archivos.
#include <sstream>  // Incluye la biblioteca para manipulación de strings como streams.
using namespace std; // Usa el espacio de nombres estándar para evitar escribir std::

// Estructura para un nodo del arbol genealogico
struct Nodo {
    string nombreCompleto;        // Nombre completo del miembro.
    string genero;                // Género del miembro.
    string relacionFamiliar;      // Relación familiar (hijo, padre, etc).
    Nodo* padre;                  // Puntero al padre.
    Nodo* hijo;                   // Puntero al primer hijo.
    Nodo* hermano;                // Puntero al siguiente hermano.
    
    // Constructor del nodo, inicializa los campos y punteros.
    Nodo(string nombre, string gen, string relacion, Nodo* p = NULL)
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), padre(p), hijo(NULL), hermano(NULL) {}
};

// Implementación de funciones (ahora definidas antes de main())

// Busca un miembro por nombre en el árbol, recursivamente.
Nodo* buscarMiembro(Nodo* raiz, const string& nombre) {
    if (raiz == NULL) return NULL; // Si el nodo es nulo, retorna NULL.
    if (raiz->nombreCompleto == nombre) // Si el nombre coincide, retorna el nodo.
        return raiz;
    
    Nodo* encontrado = buscarMiembro(raiz->hijo, nombre); // Busca en los hijos.
    if (encontrado != NULL) return encontrado; // Si lo encuentra en los hijos, retorna el nodo.
    
    return buscarMiembro(raiz->hermano, nombre); // Si no, busca en los hermanos.
}

// Muestra los ancestros de un nodo, desde la raíz hasta el padre inmediato.
void mostrarAncestros(Nodo* nodo) {
    if (nodo == NULL || nodo->padre == NULL) return; // Si el nodo o su padre es nulo, termina.
    
    mostrarAncestros(nodo->padre); // Llama recursivamente para mostrar ancestros superiores.
    cout << nodo->padre->nombreCompleto; // Muestra el nombre del padre.
    if (nodo->padre->padre != NULL) { // Si hay más ancestros, imprime flecha.
        cout << " -> ";
    }
}

// Muestra recursivamente los descendientes de un nodo.
void mostrarDescendientesRec(Nodo* nodo) {
    if (nodo == NULL) return; // Si el nodo es nulo, termina.
    
    cout << "- " << nodo->nombreCompleto << " (" << nodo->relacionFamiliar << ")" << endl; // Muestra el nombre y relación.
    mostrarDescendientesRec(nodo->hijo); // Llama recursivamente para mostrar los hijos.
    mostrarDescendientesRec(nodo->hermano); // Llama recursivamente para mostrar los hermanos.
}

// Busca un miembro y muestra sus descendientes.
void mostrarDescendientes(Nodo* raiz, const string& nombre) {
    Nodo* miembro = buscarMiembro(raiz, nombre); // Busca el miembro por nombre.
    if (miembro == NULL) { // Si no lo encuentra, muestra mensaje.
        cout << "Miembro no encontrado." << endl;
        return;
    }
    
    cout << "\n=== Descendientes de " << nombre << " ===" << endl; // Encabezado.
    if (miembro->hijo == NULL) { // Si no tiene hijos, lo indica.
        cout << "No tiene descendientes." << endl;
    } else {
        mostrarDescendientesRec(miembro->hijo); // Muestra los descendientes.
    }
}

// Visualiza el árbol en forma jerárquica.
void visualizarArbol(Nodo* nodo, string prefijo = "", bool esUltimo = true) {
    if (nodo == NULL) return; // Si el nodo es nulo, termina.
    
    cout << prefijo; // Imprime el prefijo para la jerarquía.
    
    // Imprime el símbolo de rama.
    if (esUltimo) {
        cout << "+-- ";
    } else {
        cout << "+-- ";
    }
    
    cout << nodo->nombreCompleto << " (" << nodo->genero << ", " << nodo->relacionFamiliar << ")" << endl; // Muestra datos del nodo.
    
    string nuevoPrefijo;
    if (esUltimo) {
        nuevoPrefijo = prefijo + "    "; // Espaciado para el último hijo.
    } else {
        nuevoPrefijo = prefijo + "¦   "; // Espaciado para otros hijos.
    }
    
    Nodo* hijo = nodo->hijo; // Empieza con el primer hijo.
    while (hijo != NULL) {
        bool ultimoHijo = (hijo->hermano == NULL); // Verifica si es el último hijo.
        visualizarArbol(hijo, nuevoPrefijo, ultimoHijo); // Llama recursivamente.
        hijo = hijo->hermano; // Pasa al siguiente hermano.
    }
}

// Guarda un nodo y sus descendientes en un archivo.
void guardarNodo(Nodo* nodo, ofstream& archivo) {
    if (nodo == NULL) return;
    
    archivo << nodo->nombreCompleto << "|" << nodo->genero << "|" 
            << nodo->relacionFamiliar << "|";
    if (nodo->padre != NULL) {
        archivo << nodo->padre->nombreCompleto;
    }
    archivo << endl;
    
    guardarNodo(nodo->hijo, archivo);
    guardarNodo(nodo->hermano, archivo);
}

// Guarda todo el árbol en un archivo de texto.
void guardarArbolEnArchivo(Nodo* raiz) {
    ofstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo para guardar." << endl;
        return;
    }
    guardarNodo(raiz, archivo);
    archivo.close();
    cout << "Árbol guardado correctamente en 'arbol.txt'." << endl;
}

// Carga el árbol desde un archivo de texto.
void cargarArbolDesdeArchivo(Nodo*& raiz) {
    ifstream archivo("arbol.txt");
    if (!archivo.is_open()) {
        cout << "No se encontró archivo para cargar. Se creará uno nuevo al guardar." << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string nombre, genero, relacion, nombrePadre;
        
        getline(ss, nombre, '|');
        getline(ss, genero, '|');
        getline(ss, relacion, '|');
        getline(ss, nombrePadre);
        
        if (raiz == NULL) {
            raiz = new Nodo(nombre, genero, relacion);
        } else {
            Nodo* padre = nombrePadre.empty() ? NULL : buscarMiembro(raiz, nombrePadre);
            Nodo* nuevo = new Nodo(nombre, genero, relacion, padre);
            
            if (padre != NULL) {
                if (padre->hijo == NULL) {
                    padre->hijo = nuevo;
                } else {
                    Nodo* temp = padre->hijo;
                    while (temp->hermano != NULL) {
                        temp = temp->hermano;
                    }
                    temp->hermano = nuevo;
                }
            } else {
                Nodo* temp = raiz;
                while (temp->hermano != NULL) {
                    temp = temp->hermano;
                }
                temp->hermano = nuevo;
            }
        }
    }

    archivo.close();
    cout << "Árbol cargado correctamente desde 'arbol.txt'." << endl;
}

// Visualiza el árbol completo con leyenda.
void visualizarArbolCompleto(Nodo* raiz) {
    if (raiz == NULL) {
        cout << "El árbol genealógico está vacío." << endl;
        return;
    }
    
    cout << "\n=== ÁRBOL GENEALÓGICO ===\n";
    cout << "Leyenda:\n";
    cout << "+-- Miembro principal\n";
    cout << "    +-- Hijo/hermano\n";
    cout << "    +-- Último hijo/hermano\n\n";
    
    visualizarArbol(raiz);
    
    cout << "\n";
}

// Verifica si un miembro pertenece a una rama específica.
bool verificarPertenenciaARama(Nodo* raizRama, const string& nombreMiembro) {
    if (raizRama == NULL) return false;
    if (raizRama->nombreCompleto == nombreMiembro) return true;
    
    Nodo* hijo = raizRama->hijo;
    while (hijo != NULL) {
        if (verificarPertenenciaARama(hijo, nombreMiembro)) {
            return true;
        }
        hijo = hijo->hermano;
    }
    return false;
}

// Agrega un nuevo miembro al árbol genealógico.
void agregarMiembro(Nodo*& raiz) {
    string nombre, genero, relacion, nombrePadre;
    
    cout << "Ingrese el nombre completo (sin espacios): "; cin >> nombre;
    cout << "Ingrese el género: "; cin >> genero;
    cout << "Ingrese la relación familiar: "; cin >> relacion;
    
    if (raiz == NULL) {
        raiz = new Nodo(nombre, genero, relacion);
        cout << "Miembro agregado como raíz del árbol." << endl;
        return;
    }
    
    cout << "Ingrese el nombre del padre/madre (escriba 'ninguno' si no tiene): "; cin >> nombrePadre;

    if (nombrePadre == "ninguno") {
        Nodo* nuevo = new Nodo(nombre, genero, relacion);
        Nodo* temp = raiz;
        while (temp->hermano != NULL) {
            temp = temp->hermano;
        }
        temp->hermano = nuevo;
        cout << "Miembro agregado como familiar extendido." << endl;
    } else {
        Nodo* padre = buscarMiembro(raiz, nombrePadre);
        if (padre == NULL) {
            cout << "Padre/Madre no encontrado. Miembro no agregado." << endl;
            return;
        }
        
        Nodo* nuevo = new Nodo(nombre, genero, relacion, padre);
        
        if (padre->hijo == NULL) {
            padre->hijo = nuevo;
        } else {
            Nodo* temp = padre->hijo;
            while (temp->hermano != NULL) {
                temp = temp->hermano;
            }
            temp->hermano = nuevo;
        }
        cout << "Miembro agregado correctamente." << endl;
    }
}

// Elimina un miembro del árbol genealógico.
bool eliminarMiembro(Nodo*& raiz, const string& nombre) {
    if (raiz == NULL) return false;
    
    Nodo* nodo = buscarMiembro(raiz, nombre);
    if (nodo == NULL) return false;
    
    if (nodo == raiz) {
        if (nodo->hijo == NULL && nodo->hermano == NULL) {
            delete nodo;
            raiz = NULL;
        } else {
            cout << "No se puede eliminar la raíz con descendientes." << endl;
            return false;
        }
        return true;
    }
    
    Nodo* padre = nodo->padre;
    
    if (padre != NULL && padre->hijo == nodo) {
        if (nodo->hijo != NULL) {
            Nodo* temp = nodo->hijo;
            while (temp != NULL) {
                temp->padre = padre;
                temp = temp->hermano;
            }
            temp = nodo->hijo;
            while (temp->hermano != NULL) {
                temp = temp->hermano;
            }
            temp->hermano = padre->hijo->hermano;
            padre->hijo = nodo->hijo;
        } else {
            padre->hijo = nodo->hermano;
        }
        delete nodo;
        return true;
    }
    
    Nodo* anterior = padre != NULL ? padre->hijo : raiz;
    while (anterior != NULL && anterior->hermano != nodo) {
        anterior = anterior->hermano;
    }
    
    if (anterior != NULL) {
        if (nodo->hijo != NULL) {
            Nodo* temp = nodo->hijo;
            while (temp != NULL) {
                temp->padre = padre;
                temp = temp->hermano;
            }
            temp = nodo->hijo;
            while (temp->hermano != NULL) {
                temp = temp->hermano;
            }
            temp->hermano = nodo->hermano;
            anterior->hermano = nodo->hijo;
        } else {
            anterior->hermano = nodo->hermano;
        }
        delete nodo;
        return true;
    }
    
    return false;
}

// Solicita el nombre y busca un miembro, mostrando sus datos.
void buscarMiembroPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a buscar: "; cin >> nombre;

    Nodo* encontrado = buscarMiembro(raiz, nombre);
    if (encontrado != NULL) {
        cout << "\n=== Miembro Encontrado ===" << endl;
        cout << "Nombre: " << encontrado->nombreCompleto << endl;
        cout << "Género: " << encontrado->genero << endl;
        cout << "Relación: " << encontrado->relacionFamiliar << endl;
        if (encontrado->padre != NULL) {
            cout << "Padre/Madre: " << encontrado->padre->nombreCompleto << endl;
        }
    } else {
        cout << "Miembro no encontrado en el árbol." << endl;
    }
}

// Solicita el nombre y muestra los ancestros del miembro.
void mostrarAncestrosPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro para ver sus ancestros: "; cin >> nombre;

    Nodo* miembro = buscarMiembro(raiz, nombre);
    if (miembro == NULL) {
        cout << "Miembro no encontrado." << endl;
        return;
    }
    
    cout << "\n=== Ancestros de " << nombre << " ===" << endl;
    if (miembro->padre == NULL) {
        cout << "No tiene ancestros registrados (es la raíz del árbol)." << endl;
    } else {
        mostrarAncestros(miembro);
        cout << endl;
    }
}

// Solicita el nombre y muestra los descendientes del miembro.
void mostrarDescendientesPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro para ver sus descendientes: "; cin >> nombre;

    mostrarDescendientes(raiz, nombre);
}

// Solicita el nombre y elimina un miembro del árbol.
void eliminarMiembroPrompt(Nodo*& raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a eliminar: "; cin >> nombre;

    if (!eliminarMiembro(raiz, nombre)) {
        cout << "Miembro no encontrado en el árbol o no se pudo eliminar." << endl;
    } else {
        cout << "Miembro eliminado correctamente." << endl;
    }
}

// Solicita nombres y verifica si un miembro pertenece a la rama de un ancestro.
void verificarPertenenciaPrompt(Nodo* raiz) {
    if (raiz == NULL) {
        cout << "El árbol genealógico está vacío." << endl;
        return;
    }

    string nombreMiembro, nombreAncestro;
    
    cout << "Ingrese el nombre del miembro a verificar: "; cin >> nombreMiembro;
    cout << "Ingrese el nombre del ancestro/raíz de la rama: "; cin >> nombreAncestro;
    Nodo* ancestro = buscarMiembro(raiz, nombreAncestro);
    
    if (ancestro == NULL) {
        cout << "El ancestro '" << nombreAncestro << "' no existe en el árbol." << endl;
        return;
    }

    bool pertenece = verificarPertenenciaARama(ancestro, nombreMiembro);
    
    cout << "\n=== RESULTADO ===" << endl;
    if (pertenece) {
        cout << "El miembro '" << nombreMiembro << "' SI pertenece";
        cout << " a la rama de '" << nombreAncestro << "'." << endl;
    } else {
        cout << "El miembro '" << nombreMiembro << "' NO pertenece";
        cout << " a la rama de '" << nombreAncestro << "'." << endl;
    }
}

// Muestra el menú principal del programa.
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
    cout << "Seleccione una opción: ";
}

// Función principal (ahora al final)
int main() {
    setlocale(LC_CTYPE,"Spanish"); // Configura la consola para caracteres en español.
    Nodo* raiz = NULL;             // Puntero a la raíz del árbol genealógico.
    int opcion;                    // Variable para la opción del menú.

    cargarArbolDesdeArchivo(raiz); // Carga el árbol desde archivo si existe.

    do {
        mostrarMenu();             // Muestra el menú principal.
        cin >> opcion;             // Lee la opción del usuario.
        
        switch(opcion) {
            case 1:
                agregarMiembro(raiz); // Agrega un miembro.
                break;
            case 2:
                eliminarMiembroPrompt(raiz); // Elimina un miembro.
                break;
            case 3:
                buscarMiembroPrompt(raiz); // Busca un miembro.
                break;
            case 4:
                visualizarArbolCompleto(raiz); // Visualiza el árbol.
                break;
            case 5:
                mostrarAncestrosPrompt(raiz); // Muestra ancestros.
                break;
            case 6:
                mostrarDescendientesPrompt(raiz); // Muestra descendientes.
                break;
            case 7:
                verificarPertenenciaPrompt(raiz); // Verifica pertenencia a rama.
                break;
            case 0:
                guardarArbolEnArchivo(raiz); // Guarda el árbol antes de salir.
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }
        cout << endl;
    } while(opcion != 0);

    return 0;
}
