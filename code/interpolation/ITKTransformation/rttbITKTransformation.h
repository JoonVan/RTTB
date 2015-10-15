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
// @version $Revision: 484 $ (last changed revision)
// @date    $Date: 2014-03-26 16:16:16 +0100 (Mi, 26 Mrz 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#ifndef __ITK_MAPPABLE_DOSE_ACCESSOR_H
#define __ITK_MAPPABLE_DOSE_ACCESSOR_H

#include <boost/shared_ptr.hpp>

#include "itkTransform.h"

#include "rttbDoseAccessorInterface.h"
#include "rttbTransformationInterface.h"

namespace rttb
{
	namespace interpolation
	{
		/*! @class ITKMappableDoseAccessor
		@brief This class can deal with dose information that has to be transformed into another geometry than the original dose image (transformation specified by ITK transformation object).
		*/
		class ITKTransformation: public TransformationInterface
		{
		public:
			static const unsigned int InputDimension3D = 3;
			static const unsigned int OutputDimension3D = 3;
			typedef double TransformScalarType;
			typedef itk::Transform<TransformScalarType, InputDimension3D, OutputDimension3D> Transform3D3DType;
			typedef Transform3D3DType::InputPointType InputPointType;
			typedef Transform3D3DType::OutputPointType OutputPointType;
			typedef boost::shared_ptr<ITKTransformation> Pointer;

		private:
			//! Has to be a Pointer type because of inheritance issues with itkSmartPointer (that doesn't recognize the inheritance)
			const Transform3D3DType* _pTransformation;

		protected:
			void convert(const WorldCoordinate3D& aWorldCoordinate, InputPointType& aInputPoint) const;
			void convert(const OutputPointType& aOutputPoint, WorldCoordinate3D& aWorldCoordinate) const;

		public:
			/*! @brief Constructor.
				@param aRegistration registration given in MatchPoint format (note the pointer format since itkSmartPointer does not support inheritance)
				@sa MappableDoseAccessorBase
				@pre all input parameters have to be valid
				@exception core::NullPointerException if one input parameter is NULL
				@exception core::PaddingException if the transformation is undefined and if _acceptPadding==false
			*/
			ITKTransformation(const Transform3D3DType* aTransformation);

			~ITKTransformation() {};

			/*! @brief performs a transformation targetImage --> movingImage
			*/
			bool transformInverse(const WorldCoordinate3D& worldCoordinateTarget,
			                      WorldCoordinate3D& worldCoordinateMoving) const;
			/*! @brief performs a transformation movingImage --> targetImage
			*/
			bool transform(const WorldCoordinate3D& worldCoordinateMoving,
			               WorldCoordinate3D& worldCoordinateTarget) const;
		};
	}
}

#endif