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
 * @file Implementation of EpiOutputFileViewer.cpp
 */

#include "EpiOutputFileViewer.h"

#include "viewers/EpiOutputHDF5.h"
#include "viewers/EpiOutputJSON.h"
#include "viewers/EpiOutputProto.h"

#include "calendar/Calendar.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"

using namespace std;
using namespace stride::sim_event;

namespace stride {
namespace viewers {

EpiOutputFileViewer::EpiOutputFileViewer(std::shared_ptr<SimRunner> runner, const std::string& output_prefix)
    : m_epioutput_file(), m_runner(std::move(runner)), m_interval(1)
{
        // Initialise EpiOutputFile with the right type
        std::string filetype = m_runner->GetConfig().get<string>("run.output_epi_type");
        if (filetype == "json") {
                m_epioutput_file = std::make_unique<output::EpiOutputJSON>(output_prefix);
        } else if (filetype == "hdf5") {
                m_epioutput_file = std::make_unique<output::EpiOutputHDF5>(output_prefix);
        } else if (filetype == "proto") {
                m_epioutput_file = std::make_unique<output::EpiOutputProto>(output_prefix);
        } else {
                throw std::runtime_error{"Invalid EpiOutput format specified in configuration."};
        }

        m_interval = m_runner->GetConfig().get<int>("run.output_epi_interval", 1);
}

void EpiOutputFileViewer::Update(const sim_event::Id id)
{
        const auto sim = m_runner->GetSim();
        switch (id) {
        case Id::Stepped: {
                int sim_day = sim->GetCalendar()->GetSimulationDay();
                if (sim_day % m_interval == 0) {
                        m_epioutput_file->Update(sim->GetPopulation());
                }
                break;
        }
        case Id::Finished: {
                m_epioutput_file->Finish();
                break;
        }
        default: break;
        }
}

} // namespace viewers
} // namespace stride
