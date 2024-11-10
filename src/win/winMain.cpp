#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "winMain.h"
#include "ui_winMain.h"
#include "log.h"

#ifdef WIN_MAIN_H

#define BUFFER_SIZE 1024

winMain::winMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::winMain)
{
    LogTr("Enter winMain function.");

    ui->setupUi(this);
    vidInitUi();

    LogTr("Exit winMain function.");
}

winMain::~winMain()
{
    LogTr("Enter ~winMain function.");

    delete ui;

    LogTr("Exit ~winMain function.");
}

void winMain::vidInitUi(void)
{
    LogTr("Enter vidInitUi function.");

    char buffer[BUFFER_SIZE] = {0};
    int SvrFd = 0;
    int NewSkt = 0;
    struct sockaddr_in stSvrAdr = {0};
    struct sockaddr_in stCltAdr = {0};
    socklen_t SktLen = sizeof(struct sockaddr_in);

    // Create tcp socket.
    SvrFd = socket(AF_INET, SOCK_STREAM, 0);

    if(SvrFd != 0)
    {
        LogScs("Successfully created server TCP socket.");
    }
    else
    {
        LogErr("Failed to create server TCP socket.");
    }

    // Config IP address.
    stSvrAdr.sin_family = AF_INET;
    stSvrAdr.sin_addr.s_addr = INADDR_ANY;
    stSvrAdr.sin_port = htons(12345);

    // Bind tcp socket.
    if(bind(SvrFd, (struct sockaddr*)&stSvrAdr, sizeof(stSvrAdr)) >= 0)
    {
        LogScs("Successfully bind server TCP socket.");
    }
    else
    {
        LogErr("Failed to bind server TCP socket.");
    }

    // Listen tcp socket.
    if(listen(SvrFd, 3) >= 0)
    {
        LogScs("Successfully bind server TCP socket.");
    }
    else
    {
        LogErr("Failed to bind server TCP socket.");
    }

    // Accept connect.
    NewSkt = accept(SvrFd, (struct sockaddr*)&stCltAdr, &SktLen);

    if(NewSkt >= 0)
    {
        LogScs("Successfully received socket connection.");
    }
    else
    {
        LogErr("Failed to receive socket connection.");
    }

    while(true)
    {
        // Read client message.
        read(NewSkt, buffer, BUFFER_SIZE);
        LogInf("Client: %s", buffer);
        std::string hello = "Hello from server";
        send(NewSkt, hello.c_str(), hello.size(), 0);
        LogInf("Server: %s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }

    // close(NewSkt);
    // clsoe(SvrFd);

    LogTr("Exit vidInitUi function.");
}

#endif // WIN_MAIN_H
