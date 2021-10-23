#ifndef CAEP_CIDRMASK_H
#define CAEP_CIDRMASK_H

#include "./byte.h"
#include "./IPMask.h"
#include "./IP.h"

namespace caep {

IPMask CIDRMask(int ones, int bits);

} // namespace caep 

#endif
