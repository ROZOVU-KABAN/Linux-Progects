#include "../include/SignalHandler.h"

namespace projects::signal {

SignalHandler::SignalHandler()
{
    action_.sa_handler = Handler;
    sigemptyset(&action_.sa_mask);
}

void Handler(int signalCode)
{
    switch(signalCode)
    {
    case SIGINT:
    {
       SignalHandler::GetInstance().StopEvent();
       break;
    }

    default:
    {
        throw ("Давай по новой, сигнал хуйня");
    }

    }
}

void SignalHandler::StopEvent()
{
    sigaction(SIGINT, &action_, NULL);
}


} // namesapce project::signal
