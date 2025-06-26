#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Estructura para un nodo del arbol genealogico
struct Nodo {
    string nombreCompleto;
    string genero;
    string relacionFamiliar;
    Nodo* padre;    // Puntero al padre
    Nodo* hijo;     // Puntero al primer hijo
    Nodo* hermano;  // Puntero al siguiente hermano
    
    Nodo(string nombre, string gen, string relacion, Nodo* p = NULL)
        : nombreCompleto(nombre), genero(gen), relacionFamiliar(relacion), padre(p), hijo(NULL), hermano(NULL) {}
};

// Implementación de funciones (ahora definidas antes de main())

Nodo* buscarMiembro(Nodo* raiz, const string& nombre) {
    if (raiz == NULL) return NULL;
    if (raiz->nombreCompleto == nombre) 
	return raiz;
    
    Nodo* encontrado = buscarMiembro(raiz->hijo, nombre);
    if (encontrado != NULL) return encontrado;
    
    return buscarMiembro(raiz->hermano, nombre);
}

void mostrarAncestros(Nodo* nodo) {
    if (nodo == NULL || nodo->padre == NULL) return;
    
    mostrarAncestros(nodo->padre);
    cout << nodo->padre->nombreCompleto;
    if (nodo->padre->padre != NULL) {
        cout << " -> ";
    }
}

void mostrarDescendientesRec(Nodo* nodo) {
    if (nodo == NULL) return;
    
    cout << "- " << nodo->nombreCompleto << " (" << nodo->relacionFamiliar << ")" << endl;
    mostrarDescendientesRec(nodo->hijo);
    mostrarDescendientesRec(nodo->hermano);
}

void mostrarDescendientes(Nodo* raiz, const string& nombre) {
    Nodo* miembro = buscarMiembro(raiz, nombre);
    if (miembro == NULL) {
        cout << "Miembro no encontrado." << endl;
        return;
    }
    
    cout << "\n=== Descendientes de " << nombre << " ===" << endl;
    if (miembro->hijo == NULL) {
        cout << "No tiene descendientes." << endl;
    } else {
        mostrarDescendientesRec(miembro->hijo);
    }
}

void visualizarArbol(Nodo* nodo, string prefijo = "", bool esUltimo = true) {
    if (nodo == NULL) return;
    
    cout << prefijo;
    
    // Reemplazo del operador ternario
    if (esUltimo) {
        cout << "+-- ";
    } else {
        cout << "+-- ";
    }
    
    cout << nodo->nombreCompleto << " (" << nodo->genero << ", " << nodo->relacionFamiliar << ")" << endl;
    
    string nuevoPrefijo;
    if (esUltimo) {
        nuevoPrefijo = prefijo + "    ";
    } else {
        nuevoPrefijo = prefijo + "¦   ";
    }
    
    Nodo* hijo = nodo->hijo;
    while (hijo != NULL) {
        bool ultimoHijo = (hijo->hermano == NULL);
        visualizarArbol(hijo, nuevoPrefijo, ultimoHijo);
        hijo = hijo->hermano;
    }
}

void recorridoInorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorridoInorden(raiz->hijo);
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
    Nodo* hermano = raiz->hermano;
    while (hermano != NULL) {
        recorridoInorden(hermano);
        hermano = hermano->hermano;
    }
}

void recorridoPreorden(Nodo* raiz) {
    if (raiz == NULL) return;
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
    recorridoPreorden(raiz->hijo);
    recorridoPreorden(raiz->hermano);
}

void recorridoPostorden(Nodo* raiz) {
    if (raiz == NULL) return;
    recorridoPostorden(raiz->hijo);
    cout << raiz->nombreCompleto << " (" << raiz->relacionFamiliar << ", " << raiz->genero << ")" << endl;
    recorridoPostorden(raiz->hermano);
}

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

void mostrarDescendientesPrompt(Nodo* raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro para ver sus descendientes: "; cin >> nombre;

    mostrarDescendientes(raiz, nombre);
}

void eliminarMiembroPrompt(Nodo*& raiz) {
    string nombre;
    cout << "Ingrese el nombre completo del miembro a eliminar: "; cin >> nombre;

    if (!eliminarMiembro(raiz, nombre)) {
        cout << "Miembro no encontrado en el árbol o no se pudo eliminar." << endl;
    } else {
        cout << "Miembro eliminado correctamente." << endl;
    }
}

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
	setlocale(LC_CTYPE,"Spanish");
    Nodo* raiz = NULL;
    int opcion;

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
                buscarMiembroPrompt(raiz);
                break;
            case 4:
                visualizarArbolCompleto(raiz);
                break;
            case 5:
                mostrarAncestrosPrompt(raiz);
                break;
            case 6:
                mostrarDescendientesPrompt(raiz);
                break;
            case 7:
                verificarPertenenciaPrompt(raiz);
                break;
            case 0:
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
