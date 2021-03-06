
#include "AmobaServer.hpp"
#include "SampleAmobaClient.hpp"
#include "KucsmaAmobaClient.hpp"
#include "BelaAmobaClient.hpp"
#include "ManualAmobaClient.hpp"

int main(int argc, char* argv[]) {

    AmobaServer server;
    //server.addClient(std::make_shared<KucsmaAmobaClient>());
    server.addClient(std::make_shared<ManualAmobaClient>());
    server.addClient(std::make_shared<ManualAmobaClient>());
    server.run(20, 20);
    return 0;
}
