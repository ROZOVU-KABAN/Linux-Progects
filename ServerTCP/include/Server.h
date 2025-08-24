#ifndef __SERVER_H__
#define __SERVER_H__

#include "pch.h"

namespace projects::server::tcp {

struct Config 
{
    // порт подключения 
    uint16_t port       = 8080;
    // ip подключения
    std::string address = "0.0.0.0";
    // максимальное количество подключений
    size_t maxConnect   = 1000;
    // максимальный размер очереди на подключение
    int maxQue = SOMAXCONN;
    // максимальные таймауты перед ошибкой
    int recvTimeoutMs   = 5000;
    int sendTimeoutMs   = 5000;
};

class Server
{
public:
    Server(const Config& config);
    ~Server();
    
    // копирование
    Server(const Server&)            = delete;
    Server& operator=(const Server&) = delete;
    // перемещение
    Server(Server&&)                 = delete;
    Server& operator=(Server&&)      = delete;

    void Start();
    void Stop();
    void Restart();

    bool SendData(const int client, const std::span<const uint8_t> data);
    void CloseConnection(const int client);

private:
    void SetupSocket();
    void BindAndListen();

private:
    Config config_;
    int server_ = -1;
    std::atomic<bool> running_{false};
};

}   // namespace projects::server::tcp

#endif // __SERVER_H__
