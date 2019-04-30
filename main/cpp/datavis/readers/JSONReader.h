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
 * Header file for the JSONReader class.
 */

#pragma once

#include "Reader.h"

#include "util/json.hpp"

namespace stride {
namespace datavis {

/**
 * Class that reads epi-output specified in the json data format.
 */
class JSONReader : public Reader
{
public:
	/// Construct the Reader with an istream containing the file content.
	explicit JSONReader(const std::string& path) : Reader(path)
	{}

	/// Default destructor.
	virtual ~JSONReader() = default;

	/// Read the epidemiological simulation data and add it to the model.
	virtual void ReadIntoModel(Model& datamodel) const override;
private:
	///< Create a Locality object from the specified information.
	const Locality ReadLocality(const nlohmann::json& localityData) const;

	///< Create a PopCategory object from the specified information.
	const PopCategory ReadPopCategory(const nlohmann::json& popCatData) const;
};

} // namespace datavis
} // namespace stride
