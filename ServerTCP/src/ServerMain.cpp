#include <iostream>
#include "../include/pch.h"

#include "../include/SignalHandler.h"
 
 // TODO тестовые инклуды - снести
#include <thread>
#include <chrono>


int main(int argc, char* agrv[])
{
    std::cout<<"Start main\n";

    std::cout<<"Create SignalHandlerObject\n";

    SIGNAL_HANDLER.RegisterHandler(SIGINT, [](int sig) {
        std::cout<<"\nSIGINT recived - shutdows init\n";
        exit(0);
        });

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<"Working...\n";
    }
    return 0;
}
