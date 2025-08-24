#include <iostream>
#include "../include/pch.h"

#include "../include/SignalWorker.h"
 
 // TODO тестовые инклуды - снести
#include <thread>
#include <chrono>


int main(int argc, char* agrv[])
{
    std::cout<<"Start main\n";

    std::cout<<"Create SignalWorkerObject\n";
    
    projects::signal::SignalWorker sigWorker;

    while(sigWorker.GetWorkStatus())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout<<"Working...\n";
    }

    return 0;
}
