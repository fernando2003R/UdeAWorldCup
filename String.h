#ifndef STRING_H
#define STRING_H

#include <iostream>

class String {
private:
    char* datos;
    int largo;

public:
    String();
    String(const char* str);
    String(const String& other);
    ~String();

    String& operator=(const String& other);
    String& operator=(const char* str);
    bool operator==(const String& other) const;
    int length() const;
    const char* c_str() const;

    friend std::ostream& operator<<(std::ostream& os, const String& str);
};

#endif