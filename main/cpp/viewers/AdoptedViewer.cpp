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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Definition of Observer for SimEvents for commandline interface usage.
 */

#include "AdoptedViewer.h"

#include "pop/Population.h"
#include "sim/Sim.h"
#include "sim/SimRunner.h"

using namespace std;
using namespace stride::sim_event;

namespace stride {
namespace viewers {

void AdoptedViewer::Update(const sim_event::Payload& p)
{
        switch (p.m_event_id) {
        case Id::AtStart: {
                const auto pop = p.m_runner->GetSim()->GetPopulation();
                m_adopted.push_back(pop->GetAdoptedCount());
                break;
        }
        case Id::Stepped: {
                const auto pop = p.m_runner->GetSim()->GetPopulation();
                m_adopted.push_back(pop->GetAdoptedCount());
                break;
        }
        case Id::Finished: {
                m_adopted_file.Print(m_adopted);
                break;
        }
        default: break;
        }
}

} // namespace viewers
} // namespace stride
