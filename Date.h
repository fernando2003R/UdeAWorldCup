#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
private:
    int dia;
    int mes;
    int anio;

    int diasEnMes() const {
        if (mes == 2) return 28;
        if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
        return 31;
    }

public:
    Date() : dia(20), mes(6), anio(2026) {}

    Date(int d, int m, int a) : dia(d), mes(m), anio(a) {}

    Date operator+(int days) const {
        Date result = *this;
        for (int i = 0; i < days; i++) {
            result.dia++;
            if (result.dia > result.diasEnMes()) {
                result.dia = 1;
                result.mes++;
                if (result.mes > 12) {
                    result.mes = 1;
                    result.anio++;
                }
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.dia << "/" << d.mes << "/" << d.anio;
        return os;
    }
};

#endif