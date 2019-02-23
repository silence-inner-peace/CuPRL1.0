#ifndef ERRORHELPER_H_H
#define ERRORHELPER_H_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include "CuPRL.h"

using namespace std;

namespace CuPRL
{
#define printError(msg) \
	{ \
		cerr <<"Error:"<<msg << " " << __FILE__ << ":" << __LINE__ << endl; \
	}

	inline void __checkCudaErrors(cudaError err, const char *file, const int line)
	{
		if (cudaSuccess != err)
		{
			cerr << file << " Line" << line <<" : CUDA Runtime API error" << int(err) << " : " << cudaGetErrorString(err) << endl;
			exit(EXIT_FAILURE);
		}
	}

#define checkCudaErrors(err) __checkCudaErrors (err, __FILE__, __LINE__)

}








#endif