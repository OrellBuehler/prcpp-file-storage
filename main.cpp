#include <iostream>
#include "Person.h"
#include "exception"
#include "FileStorage.h"

int main() {
    Person p;
    Person hanspi("Hans Peter", false, Address("Bahnhofstrasse", 6, "Windisch"));
    Person moni("Monika", true, Address("Steinackerstrasse", 43, "Windisch"));

    try {
        FileStorage fs1(std::cout);

        fs1 << "hanspi" << hanspi;
        fs1 << "moni" << moni;

        FileStorage fs2("fs.txt", FileStorage::Write);

        fs2 << "hanspi" << hanspi;
        fs2 << "moni" << moni;
    } catch (std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
