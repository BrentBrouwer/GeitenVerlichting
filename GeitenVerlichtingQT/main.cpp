#include <QCoreApplication>
#include <iostream>

#include "../../Wifi/WifiControl.h"

WifiServerControl test;

void Setup()
{
    WifiServerControl test2;
}

void Loop()
{

}

int main(int argc, char *argv[])
{
    Setup();

    do
    {
        Loop();
    }
    while (false);

    std::cout << "Test" << std::endl;
}
