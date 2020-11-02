#include <iostream>
#include <errno.h>
#include <memory>
#include "Event.h"
#include "EventLoop.h"
#include "EventClient.h"
#include "Selector.h"
#include "RecvEvent.h"
#include "ReadLineEvent.h"

int main(int argc, int **argv) {
    if (argc != 2) {
        errno(1, 0, "usage: select [IP]");
    }

    EventClient event_client(argv[1], 8080, std::make_shared<Selector>());

    event_client.EventRegister(std::make_shared<RecvEvent>(event_client.getSocketFd()));
    event_client.EventRegister(std::make_shared<ReadLineEvent>(stdin, event_client.getSocketFd()));

    event_client.EventRun();

    return 0;
}