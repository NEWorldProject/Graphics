#pragma once

#include <memory>
#include "Utility.h"

class EFX: public NonCopyable {

};

using EFXHandle = std::shared_ptr<EFX>;
