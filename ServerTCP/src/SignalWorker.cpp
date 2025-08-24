#include "../include/pch.h"
#include "../include/SignalWorker.h"

namespace projects::signal {

void SignalWorker::SigIntHandler(int signal)
{
    std::cout<<"Ctrl-C event was handeled. StopWork\n";
    running_ = 0;
}

void SignalWorker::SigTermHandler(int signal)
{
    std::cout<<"Terminate event was handeled. StopWork\n";
    running_ = 0;
}

SignalWorker::SignalWorker()
{
    // CTRL-C
    SIGNAL_HANDLER.RegisterHandler(SIGINT, [this](int signal) {
       return this->SigIntHandler(signal);
    });

    // TERMINATE
    SIGNAL_HANDLER.RegisterHandler(SIGTERM, [this](int signal) {
        return this->SigTermHandler(signal);
    });
}

bool SignalWorker::GetWorkStatus()
{
    return running_;
}


SignalWorker::~SignalWorker()
{
    running_ = 0;
    std::cout<<"SignalWorker stop\n";
}

} // namespace projects::signal
