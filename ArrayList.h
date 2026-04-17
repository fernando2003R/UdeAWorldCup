#ifndef ARRAYLIST_H
#define ARRAYLIST_H

template <typename T>
class ArrayList {
private:
    T* data;
    int capacidad;
    int cantidad;

    void redimensionar(int nuevaCapacidad) {
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
            nuevo[i] = data[i];
        }
        delete[] data;
        data = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    ArrayList() : data(nullptr), capacidad(0), cantidad(0) {}

    ~ArrayList() {
        delete[] data;
    }

    void add(const T& elemento) {
        if (cantidad >= capacidad) {
            redimensionar(capacidad == 0 ? 10 : capacidad * 2);
        }
        data[cantidad++] = elemento;
    }

    T& get(int index) {
        return data[index];
    }

    const T& get(int index) const {
        return data[index];
    }

    void set(int index, const T& elemento) {
        if (index >= 0 && index < cantidad) {
            data[index] = elemento;
        }
    }

    int size() const {
        return cantidad;
    }

    bool isEmpty() const {
        return cantidad == 0;
    }

    void clear() {
        cantidad = 0;
    }
};

#endif