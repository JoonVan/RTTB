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

#include <deque>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVH.h"
#include "DummyDoseAccessor.h"
#include "DummyMaskAccessor.h"

namespace rttb
{

	namespace testing
	{

		typedef core::DVH::DataDifferentialType DataDifferentialType;

		/*! @brief DVHTest - test the API of DVH
		1) test constructors (values as expected?)
		2) test assignement
		3) test set/getLabel
		4) test set/get<Values>
		5) test equality
		*/
		int DVHTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//generate artificial DVH and corresponding statistical values
			DoseTypeGy binSize = DoseTypeGy(0.1);
			DoseVoxelVolumeType voxelVolume = 8;

			DataDifferentialType anEmptyDataDifferential;
			DataDifferentialType aDataDifferential;
			DataDifferentialType aDataDifferential2;
			DataDifferentialType aDataDifferentialRelative;
			DoseStatisticType maximum = 0;
			DoseStatisticType minimum = 0;
			double sum = 0;
			double squareSum = 0;
			DoseCalcType value = 0;
			DVHVoxelNumber numberOfVoxels = 0;

			// creat default values [0,100)
			for (int i = 0; i < 100; i++)
			{
				value = DoseCalcType((double(rand()) / RAND_MAX) * 1000);
				numberOfVoxels += value;
				aDataDifferential.push_back(value);
				aDataDifferential2.push_back(value * 10);

				if (value > 0)
				{
					maximum = (i + 0.5) * binSize;

					if (minimum == 0)
					{
						minimum = (i + 0.5) * binSize;
					}
				}

				sum += value * (i + 0.5) * binSize;
				squareSum += value * pow((i + 0.5) * binSize, 2);
			}

      double medianVoxel = 0;
      double modalVoxel = 0;
      unsigned int count = 0;
      unsigned int medianCount = 0;
      unsigned int modalCount = 0;
      for (const auto& aValue : aDataDifferential) {
		std::cout << aValue << std::endl;
        if (medianVoxel < numberOfVoxels - medianVoxel) {
          medianVoxel += aValue;
          medianCount = count;
        }
        if (modalVoxel < aValue) {
          modalVoxel = aValue;
          modalCount = count;
        }
        count++;
      }
      DoseStatisticType median = (medianCount + 0.5)*binSize;
      DoseStatisticType modal = (modalCount + 0.5)*binSize;

			DoseStatisticType mean = sum / numberOfVoxels;
			DoseStatisticType variance = (squareSum / numberOfVoxels - mean * mean);
			DoseStatisticType stdDeviation = pow(variance, 0.5);

			std::deque<DoseCalcType>::iterator it;

			for (it = aDataDifferential.begin(); it != aDataDifferential.end(); ++it)
			{
				aDataDifferentialRelative.push_back((*it) / numberOfVoxels);
			}

			IDType structureID = "myStructure";
			IDType doseID = "myDose";
			IDType voxelizationID = "myVoxelization";

			//1) test default constructor (values as expected?)
			CHECK_THROW(core::DVH(anEmptyDataDifferential, binSize, voxelVolume, structureID, doseID,
			                      voxelizationID));
			CHECK_THROW(core::DVH(anEmptyDataDifferential, binSize, voxelVolume, structureID, doseID));
			CHECK_NO_THROW(core::DVH(aDataDifferential, binSize, voxelVolume, structureID, doseID,
			                         voxelizationID));
			CHECK_NO_THROW(core::DVH(aDataDifferential, binSize, voxelVolume, structureID, doseID));
			CHECK_THROW(core::DVH(aDataDifferential, 0, voxelVolume, structureID, doseID, voxelizationID));
			CHECK_THROW(core::DVH(aDataDifferential, 0, voxelVolume, structureID, doseID));
			CHECK_THROW(core::DVH(aDataDifferential, binSize, 0, structureID, doseID, voxelizationID));
			CHECK_THROW(core::DVH(aDataDifferential, binSize, 0, structureID, doseID));

			//2) test assignment
			core::DVH myTestDVH(aDataDifferential, binSize, voxelVolume, structureID, doseID);
			CHECK_NO_THROW(core::DVH myOtherDVH = myTestDVH);

			core::DVH myOtherDVH(myTestDVH);

			CHECK_NO_THROW(myOtherDVH = myTestDVH);

			//3) test set/getLabel
			core::DVH myDVH(aDataDifferential, binSize, voxelVolume, structureID, doseID, voxelizationID);

			StructureLabel label = "";
			CHECK_EQUAL(myDVH.getLabel(), label);
			CHECK_EQUAL(myDVH.getLabel(), label);
			label = "myLabel";
			CHECK_NO_THROW(myDVH.setLabel(label));
			CHECK_NO_THROW(myDVH.setLabel(label));
			CHECK_EQUAL(myDVH.getLabel(), label);
			CHECK_EQUAL(myDVH.getLabel(), label);
			label = "myLabel2";
			CHECK_NO_THROW(myDVH.setLabel(label));
			CHECK_NO_THROW(myDVH.setLabel(label));
			CHECK_EQUAL(myDVH.getLabel(), label);
			CHECK_EQUAL(myDVH.getLabel(), label);

			//4) test set/get<Values>
			//IDs
			CHECK_EQUAL(myDVH.getStructureID(), structureID);
			CHECK_EQUAL(myDVH.getDoseID(), doseID);
			CHECK_EQUAL(myDVH.getVoxelizationID(), voxelizationID);
			/*! is related to #2029*/
			myDVH.setDoseID("somethingElse");
			CHECK_EQUAL(myDVH.getDoseID(), "somethingElse");
			CHECK_EQUAL(myDVH.getVoxelizationID(), voxelizationID);
			CHECK_EQUAL(myDVH.getStructureID(), structureID);
			/*! is related to #2029*/
			myDVH.setStructureID("somethingOther");
			CHECK_EQUAL(myDVH.getDoseID(), "somethingElse");
			CHECK_EQUAL(myDVH.getVoxelizationID(), voxelizationID);
			CHECK_EQUAL(myDVH.getStructureID(), "somethingOther");


			//dataDifferential
			CHECK(myDVH.getDataDifferential() == aDataDifferential);
			CHECK(myDVH.getDataDifferential() == aDataDifferential);
			CHECK(myDVH.convertAbsoluteToRelative(false) == aDataDifferentialRelative);

			CHECK_EQUAL(myDVH.getNumberOfVoxels(), numberOfVoxels);
			CHECK_EQUAL(myDVH.getDeltaV(), voxelVolume);
			CHECK_EQUAL(myDVH.getDeltaD(), binSize);

			CHECK_EQUAL(myDVH.getMaximum(), maximum);
			CHECK_EQUAL(myDVH.getMinimum(), minimum);
			CHECK_EQUAL(myDVH.getMean(), mean);
			CHECK_EQUAL(myDVH.getVariance(), variance);
			CHECK_EQUAL(myDVH.getStdDeviation(), stdDeviation);
      CHECK_EQUAL(myDVH.getMedian(), median);
      CHECK_EQUAL(myDVH.getModal(), modal);

	//We can't test for absolute values as the implementation of rand() differs depending on compiler and different DVHs are created
      CHECK_NO_THROW(myDVH.getVx(0));
      CHECK_NO_THROW(myDVH.getVx(7));
      CHECK_NO_THROW(myDVH.getDx(0));
      CHECK_NO_THROW(myDVH.getDx(100000));


			int percentage = 20;
			VolumeType absVol = VolumeType(percentage * numberOfVoxels * voxelVolume / 100.0);
			CHECK_EQUAL(myDVH.getAbsoluteVolume(percentage), absVol);

			//5) test equality
      structureID = myDVH.getStructureID();
      doseID = myDVH.getDoseID();
      voxelizationID = myDVH.getVoxelizationID();
			core::DVH myDVH2(aDataDifferential2, binSize, voxelVolume, structureID, doseID);
      core::DVH myDVH3(aDataDifferential, binSize, voxelVolume, structureID+"_diff", doseID);
      core::DVH myDVH4(aDataDifferential, binSize, voxelVolume, structureID, doseID+"_diff");
      core::DVH myDVH5(aDataDifferential, binSize, voxelVolume, structureID, doseID, voxelizationID+"_diff");

			CHECK(!(myDVH == myDVH2));
      CHECK(!(myDVH == myDVH3));
      CHECK(!(myDVH == myDVH4));
      CHECK(!(myDVH == myDVH5));
			CHECK_EQUAL(myDVH, myDVH);
			core::DVH aDVH(myOtherDVH);
			CHECK_EQUAL(aDVH, myOtherDVH);

			std::map <DoseTypeGy, PercentType> normalizedDVHDiff = myDVH.getNormalizedDVH({ DVHType::Differential });
			for (auto elem : normalizedDVHDiff)
			{
				CHECK_EQUAL(aDataDifferential.at(std::round(elem.first / binSize)), (elem.second / voxelVolume));
			}

			std::map <DoseTypeGy, PercentType> normalizedDVHCum = myDVH.getNormalizedDVH();
			for (auto elem : normalizedDVHCum)
			{
				CHECK_EQUAL(myDVH.getDataCumulative().at(std::round(elem.first / binSize)), (elem.second / voxelVolume));
			}

      CHECK_NO_THROW(myDVH.getDataCumulative());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
