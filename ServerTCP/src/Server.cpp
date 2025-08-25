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

void Server::Start()
{
    if(running_.exchange(true))
    {
        throw std::runtime_error("Server already running");
    }

    try
    {
        SetupSocket();
        BindAndListen();

        std::cout<<"Server is started:\n\t address: "<< config_.address << "\n\t port: " <<config_.port<<"\n";
    
        //
        // TODO
        // запустить рабочие потоки
        //
        // TODO++
        // положить это на корутины
        //
    }
    catch(const std::exception& e)
    {
        running_ = false;
        throw e.what();
    }
    catch(...)
    {
        running_ = false;
        throw;
    }
}

void Server::Accept()
{
    while(running_)
    {
        sockaddr_in clientAddr = {};
        socklen_t clientLen = sizeof(clientAddr);
    
        int client = accept4(server_, 
                             reinterpret_cast<sockaddr*>(&clientAddr),
                             &clientLen, 
                             SOCK_NONBLOCK);

        if(client < 0)
        {
            if(EAGAIN == errno || EWOULDBLOCK)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            continue;
        }


        timeval tv = {};
        // принятие
        tv.tv_sec  = config_.recvTimeoutMs / 1000;
        tv.tv_usec = (config_.recvTimeoutMs % 1000) * 1000;
        setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        // отправка
        tv.tv_sec  = config_.sendTimeoutMs / 1000;
        tv.tv_usec = (config_.sendTimeoutMs % 1000) * 1000;
        setsockopt(client, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

        // TODO запустить обработчик клиента
    }
}

} // namespace projects::server::tcp
