#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <cassert>
#include <fstream>
#include <map>
#include <variant>
#include <map>

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

    std::map<Key, FileNode>& map() {
        return std::get<std::map<Key, FileNode>>(m_data);
    }

    const std::map<Key, FileNode>& map() const {
        return std::get<std::map<Key, FileNode>>(m_data);
    }

    const std::string& text() const {
        return std::get<std::string>(m_data);
    }

    void parse() {
        char delimiter;

        m_is >> std::ws; // skip whitespaces
        m_is >> delimiter; // read '{'

        while(delimiter != '}') {
            Key key;

            m_is >> key;
            key = key.substr(0, key.size() - 1); // remove ':'

            m_is >> std::ws; // skip whitespaces and check next character
            switch (m_is.peek())
            {
                case '{':
                {
                    // start a new object
                    auto [it, inserted] = map().emplace(key, FileNode(m_is));
                    if (!inserted) throw std::runtime_error("key " + key + " already exists");
                    break;
                }

                case '"':
                {
                    // read a string
                    std::string t;
                    m_is.get(); // read '"'
                    std::getline(m_is, t, '"');
                    auto [it, inserted] = map().emplace(key, FileNode(*this, t));
                    if (!inserted) throw std::runtime_error("key " + key + " already exists");
                    break;
                }

                default:
                {
                    // read value
                    std::string t;
                    m_is >> t;

                    if(t.back() == ',') {
                        t = t.substr(0, key.size() - 1);
                        m_is.unget(); // put ',' back to stream
                    }

                    auto [it, inserted] = map().emplace(key, FileNode(*this, t));
                    if (!inserted) throw std::runtime_error("key " + key + " already exists");

                    m_is >> std::ws;
                    m_is >> delimiter;
                    break;
                }
            }
        }
    }
};