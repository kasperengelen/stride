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
 * Definition of Observer for SimEvents for commandline interface usage.
 */

#include "EpiOutputFileViewer.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"

using namespace std;
using namespace stride::sim_event;

namespace stride {
namespace viewers {

EpiOutputFileViewer::EpiOutputFileViewer(std::shared_ptr<SimRunner> runner, const std::string& output_prefix) 
                : m_runner(std::move(runner))
{
                // Initialise EpiOutputFile with the right type
                std::string filetype = m_runner->GetConfig().get<string>("run.output_epi_type");
                if (filetype == "json") {
                        m_epioutput_file = std::make_unique<output::EpiOutputJSON>(output_prefix);
                }
}

void EpiOutputFileViewer::Update(const sim_event::Id id)
{
        switch (id) {
        case Id::Stepped: {
                m_epioutput_file->Update(m_runner->GetSim()->GetPopulation());
                break;
        }
        case Id::Finished: {
                m_epioutput_file->Finish(m_runner->GetSim()->GetPopulation());
                break;
        }
        default: break;
        }
}

} // namespace viewers
} // namespace stride
