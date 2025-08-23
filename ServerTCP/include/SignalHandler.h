#ifndef __SIGNAL_HANDLER__
#define __SIGNAL_HANDLER__

#include "pch.h"

namespace projects::signal {

void Handler(int signalCode);

class SignalHandler
{
public:

    SignalHandler();

    //
    // TODO
    // Подумать над деструктором
    // Также 100% объект не должен копироваться
    // Он должен только перемещаться
    //
    // TODO
    // Скорее всего его разумно сделать Singelton
    //
   
    static SignalHandler& GetInstance()
    {
        static SignalHandler handler;
        return handler;
    }
    
//
// Функции-обработчики событий
// Они будут помещены в Handler для дальнейшего вызова
//

    // Для обработки Ctrl+C
    void StopEvent();

private:

struct sigaction action_;

};

} // namespace projects::signal

#endif

