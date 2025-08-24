#include "../include/SignalHandler.h"

namespace projects::signal {

namespace {

// глобальный указатель на singleton только для внутреннего пользования
SignalHandler* g_instance = nullptr;

// маска сигналов для блокировки
sigset_t g_signalMask;

} // namespace (local)

SignalHandler::SignalHandler()
{
    if(g_instance)
    {
        throw std::runtime_error("SignalHandler already init");
    }
    
    g_instance = this;

    InitSignalMask();

    std::cout<< "SignalHandler init\n";
}

SignalHandler::~SignalHandler()
{
    running_.store(false, std::memory_order_release);
    RestoreOriginHandlers();
    g_instance = nullptr;
    std::cout<<"SignalHandlerDestructor\n";
}

void SignalHandler::InitSignalMask()
{
    sigemptyset(&g_signalMask);

    // блокировка основных сигналов
    for(int sig = 1; sig < NSIG; ++sig)
    {
        if(SIGKILL != sig && SIGSTOP != sig)
        {
            sigaddset(&g_signalMask, sig);
        }
    }
}

void SignalHandler::RegisterHandler(int signum, HandlFunc handler)
{
    if(signum <= 0 || signum >= NSIG)
    {
        throw std::invalid_argument("Invalid signal number");
    }

    //
    // Добавление нового обработчика
    //

    // TODO mutex
    struct sigaction newAction;
    newAction.sa_handler = &GlobalSignalHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = SA_RESTART; // перезапуск системных вызовов

    //
    // Сохранение старого обработчика
    //
    struct sigaction oldAction;
    if(-1 == sigaction(signum, &newAction, &oldAction))
    {
        throw std::system_error(errno, std::system_category(), "signal faild");
    }

    handlers_[signum] = {std::move(handler), oldAction, true};
}

void SignalHandler::RestoreOriginHandlers()
{
    // TODO mutex
    for(auto& [signum, info] : handlers_)
    {
        if(!info.installed)
        {
            continue;
        }

        sigaction(signum, &info.oldAction, nullptr);
        info.installed = false;
    }

    handlers_.clear();
}

void SignalHandler::GlobalSignalHandler(int signum)
{
    if(!g_instance)
    {
        return;
    }

    // TODO mutex
    auto it = g_instance->handlers_.find(signum);
    if(g_instance->handlers_.end() == it)
    {
        return;
    }

    try
    {
        it->second.handler(signum);
    }
    catch (const std::exception& e)
    { 
        std::cerr << "Exception in signal handler for signal " << signum << ": " << e.what() << std::endl;
    }
}

} // namesapce project::signal
