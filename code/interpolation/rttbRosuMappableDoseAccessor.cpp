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

#include "rttbRosuMappableDoseAccessor.h"

#include <boost/make_shared.hpp>

#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"
#include "rttbLinearInterpolation.h"

namespace rttb
{
	namespace interpolation
	{
		RosuMappableDoseAccessor::RosuMappableDoseAccessor(const core::GeometricInfo& geoInfoTargetImage,
            core::DoseAccessorInterface::ConstPointer doseMovingImage,
		        const TransformationInterface::Pointer aTransformation,
		        bool acceptPadding,
		        DoseTypeGy defaultOutsideValue): MappableDoseAccessorInterface(geoInfoTargetImage, doseMovingImage,
			                aTransformation, acceptPadding, defaultOutsideValue)
		{
			//define linear interpolation
      auto interpolationLinear = ::boost::make_shared<LinearInterpolation>();
			_spInterpolation = interpolationLinear;
			_spInterpolation->setAccessorPointer(_spOriginalDoseDataMovingImage);
		}

		GenericValueType RosuMappableDoseAccessor::getValueAt(const VoxelGridID aID) const
		{
			VoxelGridIndex3D aVoxelGridIndex3D;

			if (_geoInfoTargetImage.convert(aID, aVoxelGridIndex3D))
			{
				return getValueAt(aVoxelGridIndex3D);
			}
			else
			{
				if (_acceptPadding)
				{
					return _defaultOutsideValue;
				}
				else
				{
					throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
				}
			}
		}

		GenericValueType RosuMappableDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			//Transform requested voxel coordinates of original image into world coordinates with RTTB
			WorldCoordinate3D worldCoordinateTarget;

			if (_geoInfoTargetImage.indexToWorldCoordinate(aIndex, worldCoordinateTarget))
			{
				std::vector<WorldCoordinate3D> octants = getOctants(worldCoordinateTarget);

				if (octants.size() > 2)
				{
					DoseTypeGy interpolatedDoseValue = 0.0;

					//get trilinear interpolation value of every octant point
					for (std::vector<WorldCoordinate3D>::const_iterator octantIterator = octants.begin();
					     octantIterator != octants.end();
					     ++octantIterator)
					{
						//transform coordinates
						WorldCoordinate3D worldCoordinateMoving;
						WorldCoordinate3D worldCoordinateTargetOctant = *octantIterator;
						_spTransformation->transformInverse(worldCoordinateTargetOctant, worldCoordinateMoving);

						try
						{
							interpolatedDoseValue += _spInterpolation->getValue(worldCoordinateMoving);
						}
						//Mapped outside of image? Check if padding is allowed
						catch (core::MappingOutsideOfImageException& /*e*/)
						{
							if (_acceptPadding)
							{
								interpolatedDoseValue += _defaultOutsideValue;
							}
							else
							{
								throw core::MappingOutsideOfImageException("Mapping outside of image");
							}
						}
						catch (core::Exception& e)
						{
							std::cout << e.what() << std::endl;
							return -1;
						}
					}

					return interpolatedDoseValue / (DoseTypeGy)octants.size();
				}
				else
				{
					if (_acceptPadding)
					{
						return _defaultOutsideValue;
					}
					else
					{
						throw core::MappingOutsideOfImageException("Too many samples are mapped outside the image!");
					}
				}
			}
			else
			{
				if (_acceptPadding)
				{
					return _defaultOutsideValue;
				}
				else
				{
					throw core::MappingOutsideOfImageException("Error in conversion from index to world coordinates");
				}
			}
		}

		std::vector<WorldCoordinate3D> RosuMappableDoseAccessor::getOctants(
		    const WorldCoordinate3D& aCoordinate) const
		{
			std::vector<WorldCoordinate3D> octants;
			SpacingVectorType3D spacingTargetImage = _geoInfoTargetImage.getSpacing();

			core::GeometricInfo geometricInfoDoseData = _spOriginalDoseDataMovingImage->getGeometricInfo();

			//as the corner point is the coordinate of the voxel (grid), 0.25 and 0.75 are the center of the subvoxels
			for (double xOct = -0.25; xOct <= 0.25; xOct += 0.5)
			{
				for (double yOct = -0.25; yOct <= 0.25; yOct += 0.5)
				{
					for (double zOct = -0.25; zOct <= 0.25; zOct += 0.5)
					{
						WorldCoordinate3D aWorldCoordinate(aCoordinate.x() + (xOct * spacingTargetImage.x()),
						                                   aCoordinate.y() + (yOct * spacingTargetImage.y()),
						                                   aCoordinate.z() + (zOct * spacingTargetImage.z()));

						if (geometricInfoDoseData.isInside(aWorldCoordinate))
						{
							octants.push_back(aWorldCoordinate);
						}
					}
				}
			}

			return octants;
		}


	}//end namespace interpolation
}//end namespace rttb
