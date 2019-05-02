#pragma once
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
 *  Copyright 2019, Meyer J
 */

/**
 * @file
 * Observer for Epi-output.
 */

#include "EpiOutputFile.h"
#include "sim/event/Id.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace stride {

class SimRunner;

namespace viewers {

/// Viewer of Simulator for epi-output.
class EpiOutputFileViewer
{
public:
        /// Instantiate viewer.
        EpiOutputFileViewer(std::shared_ptr<SimRunner> runner, const std::string& output_prefix);

        /// Let viewer perform update.
        void Update(sim_event::Id id);

private:
        std::unique_ptr<output::EpiOutputFile>    m_epioutput_file;
        std::shared_ptr<SimRunner>                m_runner;
};

} // namespace viewers
} // namespace stride
