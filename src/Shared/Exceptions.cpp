#include "STILOQ/Shared/Exceptions.h"

namespace STILOQ
{
    InvalidParameterException::InvalidParameterException(char* msg) : errmsg_(msg) { }

	const char* InvalidParameterException::what()
	{
		return errmsg_;
	}   
}
