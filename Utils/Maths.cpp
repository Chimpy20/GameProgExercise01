#include "GameProgExercise01.h"
#include "Utils\Maths.h"

namespace utils
{

static unsigned int randSeed = 0;
unsigned int Rand()
{
	SYSTEMTIME time{};
	GetSystemTime( &time );
	return( ( randSeed++ * 1103515245u ) + 12345 + time.wMinute * 60 + time.wSecond );
}

} // namespace utils
