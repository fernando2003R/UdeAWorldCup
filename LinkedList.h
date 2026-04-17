#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

template <typename T>
class LinkedList {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };
    Nodo* cabeza;
    int cantidad;

public:
    LinkedList() : cabeza(nullptr), cantidad(0) {}

    ~LinkedList() {
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void add(const T& elemento) {
        Nodo* nuevo = new Nodo(elemento);
        if (!cabeza) {
            cabeza = nuevo;
        }
        else {
            Nodo* actual = cabeza;
            while (actual->siguiente) actual = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cantidad++;
    }

    // Iterador simple para recorrer
    class Iterator {
    private:
        Nodo* actual;
    public:
        Iterator(Nodo* inicio) : actual(inicio) {}
        bool hasNext() const { return actual != nullptr; }
        T& next() {
            T& dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    Iterator iterator() const { return Iterator(cabeza); }
    int size() const { return cantidad; }
};
#endif