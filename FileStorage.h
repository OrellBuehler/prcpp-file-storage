#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <cassert>
#include <functional>

class FileStorage;

using Func = std::function<FileStorage&(FileStorage&)>;

using Key = std::string;

constexpr int IndentWidth = 4;

class FileStorage {
    std::ostream& m_os;
    int m_indentLevel = 0;
    bool m_writing;
    bool m_keyExpected = true; // true: next output is a key
    bool m_firstKey = true;    // true: first key of an object

public:
    friend FileStorage& objectBegin(FileStorage& fs);
    friend FileStorage& objectEnd(FileStorage& fs);

    FileStorage() = delete;

    FileStorage(std::ostream& os) :
        m_os(os),
        m_writing(true)
    {
        objectBegin(*this);
    }

    ~FileStorage() {
        release();
    }

    FileStorage& operator<<(const std::string& s) {
        if(m_keyExpected) {
            // output a key
            if(!checkKey(s)) throw std::runtime_error("Invalid key");

            //write delimiter
            if(m_firstKey) {
                m_firstKey = false;
            } else {
                m_os << ',';
            }

            //write key
            indent();
            m_os << s << ": ";

            m_keyExpected = false;
        } else {
            // output a string value
            m_os << '"' << s << '"';
            m_keyExpected = true;
        }
        return *this;
    }

    FileStorage& operator<<(const int& i) {
        if(m_keyExpected) throw std::runtime_error("Key expected");
        m_os << i;
        m_keyExpected = true;
        return *this;
    }

    FileStorage& operator<<(Func f) {
        return f(*this);
    }

private:
    void release() {
        if (m_writing) {
            m_os << std::endl;
            objectEnd(*this);
            m_os.flush();
        }
    }

    bool checkKey(const Key& key) {
        for(auto c : key) {
            if((c < 'A' || c > 'Z') && (c < 'a' || c > 'z'))
                return false;
        }
        return true;
    }

    void indent() {
        if (m_indentLevel > 0) m_os << std::endl;
        for(int i = 0; i < m_indentLevel * IndentWidth; i++) {
            m_os << ' ';
        }
    }
};

inline FileStorage& objectBegin(FileStorage& fs) {
    fs.indent();
    fs.m_os << '{';
    fs.m_indentLevel++;
    fs.m_keyExpected = true;
    fs.m_firstKey = true;
    return fs;
}

inline FileStorage& objectEnd(FileStorage& fs) {
    assert(fs.m_keyExpected == true);
    fs.m_indentLevel--;
    fs.indent();
    fs.m_os << '}';
    fs.m_firstKey = false;
    return fs;
}