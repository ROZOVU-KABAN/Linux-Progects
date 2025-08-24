#include "../include/pch.h"
#include "../include/Server.h"

namespace projects::server::tcp {

Server::Server(const Config& config)
    : config_(config)
{
    // установить дефолтные обработчики
}

Server::~Server()
{
    Stop();
}

void Server::Stop()
{
    if(!running_.exchange(false))
    {
        return;
    }

    //
    // TODO
    // 1 - Остановить все рабочите потоки
    // 2 - Дождаться завершения всех рабочих потоков
    // 3 - закрыть сокет
    //
}

} // namespace projects::server::tcp
