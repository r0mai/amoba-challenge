
#include "AmobaServer.hpp"
#include "SampleAmobaClient.hpp"

int main(int argc, char* argv[]) {

    AmobaServer server;
    server.addClient(std::make_shared<SampleAmobaClient>());
    server.addClient(std::make_shared<SampleAmobaClient>());
    server.run(20, 20);
    return 0;
}
