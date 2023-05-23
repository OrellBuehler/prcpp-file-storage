#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <cassert>
#include <fstream>
#include <map>
#include <variant>

class FileStorage;

using Func = std::function<FileStorage&(FileStorage&)>;

using Key = std::string;

class FileNode {

};