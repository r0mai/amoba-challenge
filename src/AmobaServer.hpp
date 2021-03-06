#pragma once

#include <memory>

#include "AmobaClient.hpp"

class AmobaServer {
public:
    using ClientPtr = std::shared_ptr<AmobaClient>;
    using ClientPtrs = std::vector<ClientPtr>;

    void run(int xSize, int ySize);

    void addClient(ClientPtr client);
private:

    ClientPtrs clients;
};
