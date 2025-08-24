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

void Server::SetupSocket()
{
    server_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(-1 == server_)
    {
        throw std::system_error(errno, std::system_category(), "socket creation failed");
    }

    int opt = 1;
    if(0 > setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        close(server_);
        throw std::system_error(errno, std::system_category(), "setsockopt failed");
    }

    //
    // включение SO_REUSEPORT для мультипроцессорных систем 
    // нужно для повышения производительности
    //
    if(0 > setsockopt(server_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
    {
        // ошибка не критична
        std::cerr<< "Warning: SO_RESUSEPORT not supported\n";
    }
}

void Server::BindAndListen()
{
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config_.port);
    
    // преобразование текстового ip в машинный ip
    if(inet_pton(AF_INET, config_.address.c_str(), &serverAddr.sin_addr) <= 0)
    {
        throw std::system_error(errno, std::system_category(), "invalid bind address");
    }

    if(bind(server_, reinterpret_cast<sockaddr*>(&server_), sizeof(server_)) <= 0)
    {
        close(server_);
        throw std::system_error(errno, std::system_category(), "bind failed");
    }

    if(listen(server_, config_.maxQue) < 0)
    {
        close(server_);
        throw std::system_error(errno, std::system_category(), "listen failed");
    }
}

} // namespace projects::server::tcp
