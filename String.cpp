#define _CRT_SECURE_NO_WARNINGS
#include "String.h"
#include <cstring>

String::String() {
    datos = new char[1];
    datos[0] = '\0';
    largo = 0;
}

String::String(const char* str) {
    largo = 0;
    while (str[largo] != '\0') largo++;
    datos = new char[largo + 1];
    for (int i = 0; i <= largo; i++) {
        datos[i] = str[i];
    }
}

String::String(const String& other) {
    largo = other.largo;
    datos = new char[largo + 1];
    for (int i = 0; i <= largo; i++) {
        datos[i] = other.datos[i];
    }
}

String::~String() {
    delete[] datos;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] datos;
        largo = other.largo;
        datos = new char[largo + 1];
        for (int i = 0; i <= largo; i++) {
            datos[i] = other.datos[i];
        }
    }
    return *this;
}

String& String::operator=(const char* str) {
    delete[] datos;
    largo = 0;
    while (str[largo] != '\0') largo++;
    datos = new char[largo + 1];
    for (int i = 0; i <= largo; i++) {
        datos[i] = str[i];
    }
    return *this;
}

bool String::operator==(const String& other) const {
    if (largo != other.largo) return false;
    for (int i = 0; i <= largo; i++) {
        if (datos[i] != other.datos[i]) return false;
    }
    return true;
}

int String::length() const {
    return largo;
}

const char* String::c_str() const {
    return datos;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.datos;
    return os;
}