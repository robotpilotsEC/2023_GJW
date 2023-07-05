#include "mathematics.h"

float lowpass(float X_last, float X_new, float K)
{
	return (X_last + (X_new - X_last) * K);
}


