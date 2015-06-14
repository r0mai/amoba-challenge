
#include "AmobaServer.hpp"
#include "SampleAmobaClient.hpp"

int main(int argc, char* argv[]) {

    AmobaServer server;
    server.addClient(std::make_shared<SampleAmobaClient>());
    server.addClient(std::make_shared<SampleAmobaClient>());
    server.run(50, 70);
    return 0;
}
