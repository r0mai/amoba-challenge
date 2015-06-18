#pragma once

#include <memory>

#include "AmobaClient.hpp"

class AmobaServer {
public:
    using ClientPtr = std::shared_ptr<AmobaClient>;
    using ClientPtrs = std::vector<ClientPtr>;

    void run(bool isInfinite, int xSize = 20, int ySize = 20);

    void addClient(ClientPtr client);
private:

    ClientPtrs clients;
};
