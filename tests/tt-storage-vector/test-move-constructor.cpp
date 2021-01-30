#include <utility>
#include <iostream>

#include "tt-test.hpp"

struct Test {
    Test() {
        std::cerr << "default constructor\n";
    }

    Test(Test&& other) {
        std::cerr << "move constructor\n";
    }

    Test& operator=(Test&& other) {
        std::cerr << "move assignment operator\n";
        return *this;
    }

    ~Test() {
        std::cerr << "destructor\n";
    }

    int square(int num) {
        return num * num;
    }
};

union Union {
    Test test;
    int integer;


    Union() : test() {}
    ~Union() {
        test.~Test();
    }
};


int main() {
    int result = 0;
    {
        Test a;
        Test b = std::move(a);
        result = b.square(2);
    }

    {
        Union u;
    }

    tt_assert(result == 2);
    return 0;
}
