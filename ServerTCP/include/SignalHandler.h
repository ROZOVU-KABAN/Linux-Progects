#ifndef __SIGNAL_HANDLER_H__
#define __SIGNAL_HANDLER_H__

#include "pch.h"

#define SIGNAL_HANDLER projects::signal::SignalHandler::GetInstance()

namespace projects::signal {

class SignalHandler
{
public:
    // Копирование
    SignalHandler(const SignalHandler&)             = delete;
    SignalHandler& operator=(const SignalHandler&)  = delete;

    // Перемещение
    SignalHandler(const SignalHandler&&)        = delete;
    SignalHandler& operator=(SignalHandler&&)   = delete;
    
    // получение объекта класса
    static SignalHandler& GetInstance()
    {
        static SignalHandler handler;
        return handler;
    }

public:
    using HandlFunc = std::function<void(int)>;

    // добавление обработчика
    void RegisterHandler(int signum, HandlFunc handler);

    // удаление обработчика
    void RemoveHandler(int signum);

    // установка стандартного обработчика (восстановление)
    void SetDefaultHandler(int signum);

    //
    // TODO 
    // Функции, которые необходимо доабвить:
    // 1 - Блокировка всех сигналов в текущем потоке
    // 2 - Разблокировка сигналов в потоке
    // 3 - Ожидание сигнала
    //

private:

    SignalHandler();
    ~SignalHandler();

    static void GlobalSignalHandler(int signum);

    static void InitSignalMask();

    void RestoreOriginHandlers();

private:

    struct SignalInfo 
    {
        HandlFunc handler;
        struct sigaction oldAction;
        bool installed;
    };


private:

    std::unordered_map<int, SignalInfo> handlers_;
    std::atomic<bool> running_{true};

    // TODO добавить мьютексную синхронизацию
};

} // namespace projects::signal

#endif // __SIGNAL_HANDLER_H__

