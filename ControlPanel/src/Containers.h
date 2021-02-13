#ifndef __ELITEDUINO_CONTAINERS_H__
#define __ELITEDUINO_CONTAINERS_H__

#include "Architecture.h"

#ifdef ELITEDUINO_PROMICRO
#include <ArxContainer.h>
namespace Containers { template < typename T > using Vector = arx::vector<T>; }
#elif defined(ELITEDUINO_TEENSY)
#include <vector>
namespace Containers { template < typename T > using Vector = std::vector<T>; }
#else
#	error Unsupported platform
#endif

#endif // __ELITEDUINO_CONTAINERS_H__
