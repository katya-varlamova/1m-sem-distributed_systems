#pragma once

#include <memory>

#include "IDAFacade.h"
class IFactory
{
public:
    virtual IDAFacadePtr CreateDAFacade() = 0;
    virtual ~IFactory() = default;

};
using IFactoryPtr = std::shared_ptr<IFactory>;
