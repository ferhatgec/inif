#include "include/inif.hpp"
#include <fstream>

int main() noexcept {
    std::ifstream x("example.ini");
    std::string data;
    for(std::string temp; std::getline(x, temp); data.append(temp + "\n"))\
        ; x.close();

    inif val;

    val.parse(data);

    std::cout << val.get("name", "name") << '\n'
              << val.get("projects", "test") << '\n'
              << val.get("not", "found") << '\n';
}