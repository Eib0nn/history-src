#include <iostream>
#include <vector>

int main(){
    std::vector<int> v = {1,2,3,4};

    std::cout << "v = { ";
    int i = 0;
    while (i <= 10){
        v.push_back(i);
    }
    for (int n : v){
        std::cout << n << ", ";
    }
    std::cout << "}"; "\n";
}