// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------

#include "litCheckMacros.h"

#include "rttbDVH.h"

#ifndef __DVH_COMPARER_H
#define __DVH_COMPARER_H

namespace rttb
{

	namespace testing
	{

		typedef core::DVH::Pointer DVHPointer;

		/*! Compare 2 DVHs and return the results.
			@result Indicates if the test was successful (true) or if it failed (false)
		*/
		bool checkEqualDVH(DVHPointer aDVH1, DVHPointer aDVH2);

        DVHPointer computeDiffDVH(DVHPointer aDVH1, DVHPointer aDVH2);
	}//testing
}//rttb
#endif
