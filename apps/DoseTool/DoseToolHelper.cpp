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

#include "DoseToolHelper.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/filesystem.hpp"

#include "DoseToolApplicationData.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbDoseStatistics.h"
#include "rttbDVH.h"
#include "rttbDVHCalculator.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbDoseStatisticsXMLWriter.h"


std::vector<rttb::core::MaskAccessorInterface::Pointer>
rttb::apps::doseTool::generateMasks(
    rttb::apps::doseTool::ApplicationData& appData)
{
	std::vector<core::MaskAccessorInterface::Pointer> maskAccessorPtrVector;

	if (appData._structLoadStyle == "itk" || appData._structLoadStyle == "itkDicom") {
		maskAccessorPtrVector.push_back(rttb::io::itk::ITKImageFileMaskAccessorGenerator(appData._structFileName).generateMaskAccessor());
        appData._structNames.push_back(appData._structNameRegex);
	} else {
		if (appData._struct->getNumberOfStructures() > 0) {
			//default behavior: read only first struct that matches the regex
			unsigned int maxIterationCount = 1;

			//only if specified: read all structs that matches the regex
			if (appData._multipleStructsMode) {
				maxIterationCount = appData._struct->getNumberOfStructures();
			}

			 bool strict = !appData._allowSelfIntersection;

			for (size_t i = 0; i < maxIterationCount; i++) {
				maskAccessorPtrVector.emplace_back(boost::make_shared<rttb::masks::boost::BoostMaskAccessor>(appData._struct->getStructure(i), appData._dose->getGeometricInfo(), strict));
				maskAccessorPtrVector.at(i)->updateMask();
				appData._structNames.push_back(appData._struct->getStructure(i)->getLabel());
			}
		} else {
			std::cout << "no structures in structure set!" << std::endl;
		}
	}

	return maskAccessorPtrVector;
}

rttb::core::DoseIteratorInterface::Pointer
rttb::apps::doseTool::generateMaskedDoseIterator(
    rttb::core::MaskAccessorInterface::Pointer maskAccessorPtr,
    rttb::core::DoseAccessorInterface::Pointer doseAccessorPtr)
{
	boost::shared_ptr<core::GenericMaskedDoseIterator> maskedDoseIterator =
	    boost::make_shared<core::GenericMaskedDoseIterator>(maskAccessorPtr, doseAccessorPtr);
	rttb::core::DoseIteratorInterface::Pointer doseIterator(maskedDoseIterator);
	return doseIterator;
}

rttb::algorithms::DoseStatistics::Pointer
calculateDoseStatistics(
    rttb::core::DoseIteratorInterface::Pointer doseIterator, bool calculateComplexDoseStatistics,
    rttb::DoseTypeGy prescribedDose)
{
	rttb::algorithms::DoseStatisticsCalculator doseStatsCalculator(doseIterator);

	if (calculateComplexDoseStatistics) {
		return doseStatsCalculator.calculateDoseStatistics(prescribedDose);
	} else {
		return doseStatsCalculator.calculateDoseStatistics();
	}
}


rttb::core::DVH::Pointer calculateDVH(
    rttb::core::DoseIteratorInterface::Pointer
    doseIterator, rttb::IDType structUID, rttb::IDType doseUID)
{
	rttb::core::DVHCalculator calc(doseIterator, structUID, doseUID);
	rttb::core::DVH::Pointer dvh = calc.generateDVH();
	return dvh;
}

std::string rttb::apps::doseTool::assembleFilenameWithStruct(const std::string& originalFilename, const std::string& structName) {
    boost::filesystem::path originalFile(originalFilename);
    std::string newFilename = originalFile.stem().string() + "_" + structName + originalFile.extension().string();
    boost::filesystem::path newFile(originalFile.parent_path() / newFilename);
    return newFile.string();
}

/*! @brief Writes the dose statistics as XML to a file
@details adds a <config>....</config> part to the RTTB generated xml where the used files and struct names are stored.
*/
void writeDoseStatisticsFile(
    rttb::algorithms::DoseStatistics::Pointer statistics,
    const std::string& filename, const std::string& structName,
    rttb::apps::doseTool::ApplicationData& appData)
{

	auto doseStatisticsXMLWriter = rttb::io::other::DoseStatisticsXMLWriter();

	boost::property_tree::ptree originalTree = doseStatisticsXMLWriter.writeDoseStatistics(statistics);

	//add config part to xml
	originalTree.add("statistics.config.requestedStructRegex", appData._structNameRegex);
	originalTree.add("statistics.config.structName", structName);
	originalTree.add("statistics.config.doseUID", appData._dose->getUID());
	originalTree.add("statistics.config.doseFile", appData._doseFileName);
	originalTree.add("statistics.config.structFile", appData._structFileName);

	boost::property_tree::ptree reorderedTree, configTree, resultsTree;
	configTree = originalTree.get_child("statistics.config");
	resultsTree = originalTree.get_child("statistics.results");
	reorderedTree.add_child("statistics.config", configTree);
	reorderedTree.add_child("statistics.results", resultsTree);

	boost::property_tree::write_xml(filename, reorderedTree, std::locale(),  boost::property_tree::xml_writer_make_settings<std::string>('\t', 1));
}

void writeDVHFile(rttb::core::DVH::Pointer dvh, const std::string& filename)
{
	rttb::DVHType typeCum = { rttb::DVHType::Cumulative };
	rttb::io::other::DVHXMLFileWriter dvhWriter(filename, typeCum);
	dvhWriter.writeDVH(dvh);
}

void rttb::apps::doseTool::processData(rttb::apps::doseTool::ApplicationData& appData) {
    std::cout << std::endl << "generating masks... ";
    std::vector<core::MaskAccessorInterface::Pointer> maskAccessorPtrVector = generateMasks(
        appData);
    std::cout << "done." << std::endl;

    for (size_t i = 0; i < maskAccessorPtrVector.size(); i++) {
        core::DoseIteratorInterface::Pointer spDoseIterator(generateMaskedDoseIterator(
            maskAccessorPtrVector.at(i),
            appData._dose));

        if (appData._computeDoseStatistics) {
            std::cout << std::endl << "computing dose statistics... ";
            auto statistics = calculateDoseStatistics(
                spDoseIterator,
                appData._computeComplexDoseStatistics, appData._prescribedDose);
            std::cout << "done." << std::endl;

            std::cout << std::endl << "writing dose statistics to file... ";
            std::string outputFilename;

            if (appData._multipleStructsMode) {
                outputFilename = assembleFilenameWithStruct(appData._doseStatisticOutputFileName,
                    appData._structNames.at(i));
            } else {
                outputFilename = appData._doseStatisticOutputFileName;
            }

            writeDoseStatisticsFile(statistics, outputFilename, appData._structNames.at(i), appData);
            std::cout << "done." << std::endl;
        }

        if (appData._computeDVH) {
            std::cout << std::endl << "computing DVH... ";
            rttb::IDType structUID;
            rttb::IDType doseUID;

            //Generate random UID
            if (appData._structLoadStyle == "itk") {
                structUID = "struct.fromVoxelizedITK";
                doseUID = "dose.fromVoxelizedITK";
            } else {
                structUID = appData._struct->getUID();
                doseUID = appData._dose->getUID();
            }

            core::DVH::Pointer dvh = calculateDVH(spDoseIterator, structUID,
                doseUID);
            std::cout << "done." << std::endl;

            std::cout << std::endl << "writing DVH to file... ";
            std::string outputFilename;

            if (appData._multipleStructsMode) {
                outputFilename = assembleFilenameWithStruct(appData._dvhOutputFilename, appData._structNames.at(i));
            } else {
                outputFilename = appData._dvhOutputFilename;
            }

            writeDVHFile(dvh, outputFilename);
            std::cout << "done." << std::endl;
        }
    }
}
