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

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests


#include <math.h>

#include "CreateTestStructure.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbInvalidDoseException.h"

namespace rttb
{

	namespace testing
	{

		CreateTestStructure::~CreateTestStructure() {}

		CreateTestStructure::CreateTestStructure(const core::GeometricInfo& aGeoInfo)
		{
			_geoInfo = aGeoInfo;
		}

		PolygonType CreateTestStructure::createPolygonLeftUpper(std::vector<VoxelGridIndex2D> aVoxelVector,
		        GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);

				polygon.push_back(p1);
				std::cout << "(" << p1.x() << "," << p1.y() << "," << p1.z() << ")" << "; ";
			}

			std::cout << std::endl;
			return polygon;
		}

		PolygonType CreateTestStructure::createPolygonCenter(std::vector<VoxelGridIndex2D> aVoxelVector,
		        GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p;
				p(0) = p1.x() + delta.x() / 2;
				p(1) = p1.y() + delta.y() / 2;
				p(2) = p1.z();

				polygon.push_back(p);
			}

			std::cout << std::endl;
			return polygon;
		}

        PolygonType CreateTestStructure::createPolygonCenterOnPlaneCenter(std::vector<VoxelGridIndex2D> aVoxelVector,
            GridIndexType sliceNumber)
        {

            PolygonType polygon;

            for (size_t i = 0; i < aVoxelVector.size(); i++)
            {
                VoxelGridIndex3D voxelIndex;
                DoubleVoxelGridIndex3D indexDouble = DoubleVoxelGridIndex3D((aVoxelVector.at(i)).x(), (aVoxelVector.at(i)).y(),
                    sliceNumber);

                WorldCoordinate3D p1;
                _geoInfo.geometryCoordinateToWorldCoordinate(indexDouble, p1);
            
                polygon.push_back(p1);
		std::cout << "(" << p1.x() << "," << p1.y() << "," << p1.z() << ")" << "; ";
            }

            std::cout << std::endl;
            return polygon;
        }


		PolygonType CreateTestStructure::createPolygonBetweenUpperLeftAndCenter(
		    std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p;
				p(0) = p1.x() + delta.x() / 4;
				p(1) = p1.y() + delta.y() / 4;
				p(2) = p1.z();

				polygon.push_back(p);
				std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << ")" << "; ";
			}

			std::cout << std::endl;
			return polygon;
		}


		PolygonType CreateTestStructure::createPolygonBetweenLowerRightAndCenter(
		    std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p;
				p(0) = p1.x() + delta.x() * 0.75;
				p(1) = p1.y() + delta.y() * 0.75;
				p(2) = p1.z();

				polygon.push_back(p);
				std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << ")" << "; ";
			}

			std::cout << std::endl;
			return polygon;
		}


		PolygonType CreateTestStructure::createPolygonLeftEdgeMiddle(std::vector<VoxelGridIndex2D>
		        aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p;
				p(0) = p1.x();
				p(1) = p1.y() + delta.y() * 0.5;
				p(2) = p1.z();

				polygon.push_back(p);
				std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << ")" << "; ";
			}

			std::cout << std::endl;
			return polygon;
		}



		PolygonType CreateTestStructure::createPolygonUpperCenter(std::vector<VoxelGridIndex2D>
		        aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p;
				p(0) = p1.x() + delta.x() * 0.5;
				p(1) = p1.y();
				p(2) = p1.z();

				polygon.push_back(p);
				std::cout << "(" << p.x() << "," << p.y() << "," << p.z() << ")" << "; ";
			}

			std::cout << std::endl;
			return polygon;
		}



		PolygonType CreateTestStructure::createPolygonIntermediatePoints(std::vector<VoxelGridIndex2D>
		        aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			for (size_t i = 0; i < aVoxelVector.size(); i++)
			{
				VoxelGridIndex3D voxelIndex;
				VoxelGridIndex3D voxelIndex2;

				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				if (i < (aVoxelVector.size() - 1))
				{
					voxelIndex2(0) = (aVoxelVector.at(i + 1)).x();
					voxelIndex2(1) = (aVoxelVector.at(i + 1)).y();
					voxelIndex2(2) = sliceNumber;
				}
				else
				{
					voxelIndex2(0) = (aVoxelVector.at(0)).x();
					voxelIndex2(1) = (aVoxelVector.at(0)).y();
					voxelIndex2(2) = sliceNumber;
				}

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D p2;
				_geoInfo.indexToWorldCoordinate(voxelIndex2, p2);
				SpacingVectorType3D delta2 = _geoInfo.getSpacing();

				WorldCoordinate3D wp1;
				wp1(0) = p1.x() + delta.x() * 0.75;
				wp1(1) = p1.y() + delta.y() * 0.75;
				wp1(2) = p1.z();

				WorldCoordinate3D wp2;
				wp2(0) = p2.x() + delta.x() * 0.75;
				wp2(1) = p2.y() + delta.y() * 0.75;
				wp2(2) = p2.z();

				polygon.push_back(wp1);

				double diffX = (wp2.x() - wp1.x()) / 1000.0;
				double diffY = (wp2.y() - wp1.y()) / 1000.0;

				WorldCoordinate3D wp_intermediate;
				wp_intermediate(0) = 0;
				wp_intermediate(1) = 0;

				for (int i = 0 ; i < 1000 ; i++)
				{
					wp_intermediate(0) = wp1.x() + i * diffX;
					wp_intermediate(1) = wp1.y() + i * diffY;

					polygon.push_back(wp_intermediate);
				}
			}

			std::cout << std::endl;
			return polygon;
		}

		PolygonType CreateTestStructure::createPolygonCircle(std::vector<VoxelGridIndex2D> aVoxelVector,
		        GridIndexType sliceNumber)
		{

			PolygonType polygon;

			if (aVoxelVector.size() > 0)
			{
				int i = 0;

				VoxelGridIndex3D voxelIndex;

				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D wp1;
				wp1(0) = p1.x();
				wp1(1) = p1.y();
				wp1(2) = p1.z();

				WorldCoordinate3D wp_intermediate;
				wp_intermediate(0) = 0;
				wp_intermediate(1) = 0;
				wp_intermediate(2) = p1.z();

				double radius = 2 * delta.x();
				double frac_radius = (radius * 0.001);

				double correct_y = (delta.x() / delta.y());

				for (int i = 0 ; i <= 1000 ; i++)
				{
					double y_offset = sqrt((radius * radius) - ((frac_radius * i) * (frac_radius * i)));

					wp_intermediate(0) = wp1.x() +  frac_radius * i;
					wp_intermediate(1) = wp1.y() - (y_offset * correct_y) ;

					polygon.push_back(wp_intermediate);
				}

				for (int i = 1000 ; i >= 0 ; i--)
				{

					double y_offset = sqrt((radius * radius) - ((frac_radius * i) * (frac_radius * i))) ;

					wp_intermediate(0) = wp1.x() +  frac_radius * i;
					wp_intermediate(1) = wp1.y() + (y_offset * correct_y);

					polygon.push_back(wp_intermediate);
				}

				for (int i = 0 ; i <= 1000 ; i++)
				{
					double y_offset = sqrt((radius * radius) - ((frac_radius * i) * (frac_radius * i)));

					wp_intermediate(0) = wp1.x() - frac_radius * i;
					wp_intermediate(1) = wp1.y() + y_offset * correct_y ;

					polygon.push_back(wp_intermediate);
				}

				for (int i = 1000 ; i >= 0 ; i--)
				{
					double y_offset = sqrt((radius * radius) - ((frac_radius * i) * (frac_radius * i)));

					wp_intermediate(0) = wp1.x() - frac_radius * i ;
					wp_intermediate(1) = wp1.y() - (y_offset * correct_y);

					polygon.push_back(wp_intermediate);
				}
			}

			std::cout << std::endl;
			return polygon;
		}

		PolygonType CreateTestStructure::createStructureSeveralSectionsInsideOneVoxelA(
		    std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			if (aVoxelVector.size() > 0)
			{
				int i = 0;

				VoxelGridIndex3D voxelIndex;

				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D wp1;
				wp1(0) = p1.x();
				wp1(1) = p1.y();
				wp1(2) = p1.z();

				WorldCoordinate3D wp_intermediate;
				wp_intermediate(0) = 0;
				wp_intermediate(1) = 0;
				wp_intermediate(2) = p1.z();

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.25);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.25);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.75);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.75);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.25);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.25);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 2.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.75);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.75);
				wp_intermediate(1) = wp1.y() + (delta.y() * 3.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() ;
				wp_intermediate(1) = wp1.y() + (delta.y() * 3.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() ;
				wp_intermediate(1) = wp1.y() + (delta.y() * 3.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() ;
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.75);

				polygon.push_back(wp_intermediate);
			}

			std::cout << std::endl;
			return polygon;
		}

		PolygonType CreateTestStructure::createStructureSelfTouchingA(std::vector<VoxelGridIndex2D>
		        aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			if (aVoxelVector.size() > 0)
			{
				int i = 0;

				VoxelGridIndex3D voxelIndex;

				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D wp1;
				wp1(0) = p1.x();
				wp1(1) = p1.y();
				wp1(2) = p1.z();


				WorldCoordinate3D wp_intermediate;
				wp_intermediate(0) = 0;
				wp_intermediate(1) = 0;
				wp_intermediate(2) = p1.z();

				wp_intermediate(0) = wp1.x();
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y();

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() ;
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);
			}

			std::cout << std::endl;
			return polygon;
		}

		PolygonType CreateTestStructure::createStructureSelfTouchingB(std::vector<VoxelGridIndex2D>
		        aVoxelVector, GridIndexType sliceNumber)
		{

			PolygonType polygon;

			if (aVoxelVector.size() > 0)
			{
				int i = 0;

				VoxelGridIndex3D voxelIndex;

				voxelIndex(0) = (aVoxelVector.at(i)).x();
				voxelIndex(1) = (aVoxelVector.at(i)).y();
				voxelIndex(2) = sliceNumber;

				WorldCoordinate3D p1;
				_geoInfo.indexToWorldCoordinate(voxelIndex, p1);
				SpacingVectorType3D delta = _geoInfo.getSpacing();

				WorldCoordinate3D wp1;
				wp1(0) = p1.x();
				wp1(1) = p1.y();
				wp1(2) = p1.z();

				WorldCoordinate3D wp_intermediate;
				wp_intermediate(0) = 0;
				wp_intermediate(1) = 0;
				wp_intermediate(2) = p1.z();

				wp_intermediate(0) = wp1.x();
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y();

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 1.0);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x();
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() + (delta.x() * 0.5);
				wp_intermediate(1) = wp1.y() + (delta.y() * 1.0);

				polygon.push_back(wp_intermediate);

				wp_intermediate(0) = wp1.x() ;
				wp_intermediate(1) = wp1.y() + (delta.y() * 0.5);

				polygon.push_back(wp_intermediate);
			}

			std::cout << std::endl;
			return polygon;
		}

	}//testing
}//rttb

