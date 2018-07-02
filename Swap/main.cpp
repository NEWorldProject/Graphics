#include <iostream>
#include "GIL/RGBA.h"

int main(int argc, char** argv) {
    {
        GIL::RGBA::u8::Color test;
        test.setR(111).setG(112).setB(113).setA(255);
        std::cout << sizeof(test) << ' ' << alignof(decltype(test)) << ' '
                  << (int) test.getR() << ' ' << (int) test.getG() << ' '
                  << (int) test.getB() << ' ' << (int) test.getA() << ' '
                  << std::endl;
    }

    {
        GIL::ARGB::u8::Color test;
        test.setR(111).setG(112).setB(113).setA(255);
        std::cout << sizeof(test) << ' ' << alignof(decltype(test)) << ' '
                  << (int) test.getR() << ' ' << (int) test.getG() << ' '
                  << (int) test.getB() << ' ' << (int) test.getA() << ' '
                  << std::endl;
    }

    return 0;
}
