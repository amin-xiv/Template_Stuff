#include <iostream>
#include "string.hpp"


int main() {

    string str{};
    std::cout << "str1: " << str.data() << ", " << str.size() << ", " << str.get_current_capacity() << std::endl;
    string str2{"hello"};
    std::cout << "str2: " << str2.data() << ", " << str2.size() << ", " << str2.get_current_capacity() << std::endl;
    // string str3 = str2;
    // std::cout << "str3: " << str3.data() << ", " << str3.size() << ", " << str3.get_current_capacity() << std::endl;



    
    return 0;

}