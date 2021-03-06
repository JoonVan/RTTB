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

#ifndef __BIO_MODEL_CMD_LINE_PARSER
#define __BIO_MODEL_CMD_LINE_PARSER

#include "CmdLineParserBase.h"
namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
			/*! @class BioModelCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class BioModelCmdLineParser : public cmdlineparsing::CmdLineParserBase
			{
			public:
				BioModelCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
					const std::string& description, const std::string& contributor, const std::string& category);
				void validateInput() const;
				void printHelp() const;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
				const std::string OPTION_DOSE_FILE = "dose";
				const std::string OPTION_OUTPUT_FILE = "outputFile";
				const std::string OPTION_MODEL = "model";
				const std::string OPTION_MODEL_PARAMETERS = "modelParameters";
                const std::string OPTION_MODEL_PARAMETER_MAPS = "modelParameterMaps";
				const std::string OPTION_LOAD_STYLE = "loadStyle";
                const std::string OPTION_LOAD_STYLE_PARAMETER_MAPS = "loadStyleParameterMaps";
				const std::string OPTION_DOSE_SCALING = "doseScaling";
                const std::string OPTION_N_FRACTIONS = "nFractions";
			};

		}
	}
}

#endif