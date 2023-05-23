#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <cassert>
#include <fstream>
#include <map>
#include <variant>

class FileStorage;


using Key = std::string;

class FileNode {
    friend class FileStorage;

    std::ifstream m_ifs;
    std::istream& m_is;

    std::variant<std::map<Key, FileNode>, std::string> m_data;

public:

private:
    FileNode()
        : m_is(m_ifs)
    {}

    FileNode(const std::string& fileName, bool read)
        : m_ifs(read ? std::ifstream(fileName) : std::ifstream())
        , m_is(m_ifs)
    {
        if(read) parse();
    }

    // create inner node
    FileNode(std::istream& is)
        : m_is(is)
    {
        parse();
        assert(m_data.index() == 0);
    }

    // create leaf node
    FileNode(const FileNode& fn, const std::string& text)
        : m_is(fn.m_is)
        , m_data(text)
    {
        assert(m_data.index() == 1);
    }

    void parse() {}
};