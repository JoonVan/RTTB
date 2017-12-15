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
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#ifndef __DOSE_ACCESSOR_GENERATOR_INTERFACE_H
#define __DOSE_ACCESSOR_GENERATOR_INTERFACE_H

#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class DoseAccessorGeneratorInterface
			@brief Interface for all Dose Accessor generating classes
		*/
		class DoseAccessorGeneratorInterface
		{
		public:
			typedef core::AccessorInterface::AccessorPointer DoseAccessorPointer;


		private:
			DoseAccessorGeneratorInterface(const
			                               DoseAccessorGeneratorInterface&) = delete; //not implemented on purpose -> non-copyable
			DoseAccessorGeneratorInterface& operator=(const
			        DoseAccessorGeneratorInterface&) = delete;//not implemented on purpose -> non-copyable


		protected:
			DoseAccessorGeneratorInterface() = default;
			virtual ~DoseAccessorGeneratorInterface() = default;

		public:


			/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
			*/
			virtual DoseAccessorPointer generateDoseAccessor() = 0;
		};
	}
}

#endif
