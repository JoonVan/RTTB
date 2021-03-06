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

#include "rttbMinOCxVolumeToDoseMeasureCollectionCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		MinOCxVolumeToDoseMeasureCollectionCalculator::MinOCxVolumeToDoseMeasureCollectionCalculator(const std::vector<double>& precomputeVolumeValues,
			const VolumeType volume, const std::vector<DoseTypeGy>& doseVector, const std::vector<double>& voxelProportionVector,
			const DoseVoxelVolumeType currentVoxelVolume, const DoseStatisticType minimum, const DoseStatisticType maximum, 
			bool multiThreading) : VolumeToDoseMeasureCollectionCalculator(precomputeVolumeValues, volume,
				doseVector, voxelProportionVector, currentVoxelVolume, VolumeToDoseMeasureCollection::MinOCx, 
				multiThreading), _minimum(minimum), _maximum(maximum) {}

		DoseTypeGy MinOCxVolumeToDoseMeasureCollectionCalculator::computeSpecificValue(double xAbsolute) const
		{
			double noOfVoxel = xAbsolute / _currentVoxelVolume;
			DoseTypeGy resultDose = 0;

			double countVoxels = 0;
			auto it = _doseVector.begin();
			auto itD = _voxelProportionVector.begin();

			for (; itD != _voxelProportionVector.end(); ++itD, ++it)
			{
				countVoxels += *itD;

				if (countVoxels >= noOfVoxel)
				{
					break;
				}
			}

			if (it != _doseVector.end())
			{
				++it;

				if (it != _doseVector.end())
				{
					resultDose = *it;
				}
				else
				{
					resultDose = (DoseTypeGy)_maximum;
				}
			}
			else
			{
				resultDose = (DoseTypeGy)_minimum;
			}
			return resultDose;
		}
	}
}