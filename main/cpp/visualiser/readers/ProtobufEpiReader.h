/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2019, ACED.
 */
/**
 * @file
 * Header file for the ProtobufReader class.
 */

#pragma once

#include "EpiReader.h"

namespace stride {
namespace visualiser {

/**
 * @class ProtobufEpiReader
 * Class that reads epi-output specified in the Protobuf data format.
 */
class ProtobufEpiReader : public EpiReader
{
public:
        /**
         * Constructor.
         *
         * @param path An std::string that contains the path to the JSON file.
         */
        explicit ProtobufEpiReader(const std::string& path) : EpiReader(path) {}

        /**
         * Default destructor.
         */
        virtual ~ProtobufEpiReader() = default;

        /**
         * Read the epidemiological information contained in the file and add it to the
         * specified model.
         */
        virtual void ReadIntoModel(Model& datamodel) const override;
};

} // namespace visualiser
} // namespace stride
