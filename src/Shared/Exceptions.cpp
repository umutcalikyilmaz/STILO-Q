#include "STILO-Q/Shared/Exceptions.h"

namespace QEVO
{
    InvalidParameterException::InvalidParameterException(char* msg) : errmsg_(msg) { }

	const char* InvalidParameterException::what()
	{
		return errmsg_;
	}   
}
