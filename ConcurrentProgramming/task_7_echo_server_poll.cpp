#include <iostream>
#include <set>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define POLL_SIZE 32

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

    struct pollfd Set[POLL_SIZE];
    Set[0].fd = MasterSocket;
    Set[0].events = POLLIN;

    while(true){
        unsigned Index = 1;
        for (auto it = SlaveSockets.begin(); it != SlaveSockets.end(); ++it){
            Set[Index].fd = *it;
            Set[Index].events = POLLIN;
            ++Index;
        }

        unsigned SetSize = 1 + SlaveSockets.size();

        poll(Set, SetSize, -1);

        for (unsigned i = 0; i < SetSize; ++i){
            if (Set[i].revents & POLLIN){
                if (i){
                    static char Buf[1024];
                    int RecvSize = recv(Set[i].fd, Buf, 1024, MSG_NOSIGNAL);
                    if (RecvSize == 0 && errno != EAGAIN){
                        shutdown(Set[i].fd,SHUT_RDWR);
                        close(Set[i].fd);
                        SlaveSockets.erase(Set[i].fd);
                    }
                    else if (RecvSize > 0){
                        send(Set[i].fd,Buf,RecvSize, MSG_NOSIGNAL);
                    }
                }else{
                    int SlaveSocket = 
                        accept(MasterSocket, 0, 0);
                    set_nonblock(SlaveSocket);
                    SlaveSockets.insert(SlaveSocket);

                }
            }
        }
    }
    return 0;
}