#ifndef __SIGNAL_WORKER_H__
#define __SIGNAL_WORKER_H__
#include "pch.h"
#include "SignalHandler.h"

namespace projects::signal {

class SignalWorker
{
public:

    SignalWorker();
    ~SignalWorker();

    bool GetWorkStatus();

private:
    void SigTermHandler(int signal);
    void SigIntHandler(int signal);

private:
    volatile sig_atomic_t running_;
};

} // namesapce projects::signal

#endif // __SIGNAL_WORKER_H__
