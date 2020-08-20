#include <iostream>
#include <set>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int set_nonblock(int fd){
    int flags;
    #if defined(O_NONBLOCK)
        if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
            flags = 0;
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    #else
        flags = 1;
        return ioctl(fd, FIONBIO, &flags);
    #endif
}

int main(){
    int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::set<int> SlaveSockets;

    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(55555);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(SockAddr))){
        std::cerr << "bind error" << std::endl;
        exit(-1);
    }

    
    set_nonblock(MasterSocket);

    listen(MasterSocket, SOMAXCONN);

    std::cerr << "Initialized" << std::endl;

    while(true){
        std::cerr << "work" << std::endl;
        fd_set Set;
        FD_ZERO(&Set);
        FD_SET(MasterSocket, &Set);
        for (auto Iter = SlaveSockets.begin();
                Iter != SlaveSockets.end();
                ++Iter){
                FD_SET(*Iter, &Set);
        }
        std::cerr << "work2" << std::endl;
        int Max = std::max(MasterSocket, 
            SlaveSockets.empty() ? 0 : *(std::prev(SlaveSockets.end())));
        std::cerr << "work3" << std::endl;
        select(Max + 1, &Set, NULL, NULL , NULL);
        std::cerr << "work4" << std::endl;
        for (auto it = SlaveSockets.begin(); it != SlaveSockets.end();){
            bool inc_it = false;
            if (FD_ISSET(*it,&Set)){
                static char Buffer[1024];
                int RecvSize = recv(*it, Buffer, 1024, MSG_NOSIGNAL);
                std::cerr << "Try recv" << std::endl;
                if (RecvSize == 0 && errno != EAGAIN){
                    shutdown(*it, SHUT_RDWR);
                    close(*it);
                    it = SlaveSockets.erase(it);
                    inc_it = true;
                    std::cerr << "shutdown" << std::endl;
                }
                else if (RecvSize > 0) {
                    send(*it, Buffer, RecvSize, MSG_NOSIGNAL);
                    std::cerr << "Data sent" << std::endl;
                }
            }
            if (!inc_it)
                ++it;
        }

        if (FD_ISSET(MasterSocket, &Set)){
            int SlaveSocket = accept(MasterSocket, 0, 0);
            set_nonblock(SlaveSocket);
            SlaveSockets.insert(SlaveSocket);
            std::cerr << "New conection added" << std::endl;
        }
    }
    return 0;
}