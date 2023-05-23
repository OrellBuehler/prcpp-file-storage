#pragma once

#include <string>
#include "FileStorage.h"

class Address {
    std::string m_street;
    std::string m_city;
    int m_number;

public:
    Address() :
        m_number(0)
    {}

    Address(const std::string& street, int number, const std::string& city) :
        m_street(street),
        m_city(city),
        m_number(number)
    {}

    friend FileStorage& operator<<(FileStorage& fs, const Address& a) {
        fs << objectBegin;
        fs << "street" << a.m_street;
        fs << "number" << a.m_number;
        fs << "city" << a.m_city;
        fs << objectEnd;
        return fs;
    }
};

class Person {
    std::string m_name;
    Address m_address;
    bool m_female = false;

public:
    Person() = default;

    Person(const std::string& name, const bool m_female, const Address& address) :
        m_name(name),
        m_address(address)
    {}

    friend FileStorage& operator<<(FileStorage& fs, const Person& p) {
        fs << objectBegin;
        fs << "name" << p.m_name;
        fs << "address" << p.m_address;
        fs << "female" << p.m_female;
        fs << objectEnd;
        return fs;
    }
};