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
 *  Copyright 2019, ACED
 */

/**
 * @file Header for the EpiOutputJSON class.
 */

#pragma once

#include "viewers/EpiOutputFile.h"

#include "json.hpp"

namespace stride {
namespace output {

class EpiOutputJSON : public EpiOutputFile
{
public:
        explicit EpiOutputJSON(const std::string& output_dir = "output");

        /// Overridden update method.
        virtual void Update(std::shared_ptr<const Population> population) override;

        /// Dump json data to file.
        virtual void Finish() override;

private:
        /// Initialize json object and open file stream.
        virtual void Initialize(const std::string& output_dir) override;

private:
        nlohmann::json m_data;
};

} // namespace output
} // namespace stride
