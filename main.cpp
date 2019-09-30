#include <iostream>
#include <fstream>
#include <sstream>


int main() {
    std::ifstream input("input.txt");
    std::stringstream ss;
    ss << input.rdbuf();

    std::cout << ss.str() << std::endl;

    return 0;
}
