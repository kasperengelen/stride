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
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Implementation for SimRunner.
 */

#include "SimRunner.h"

#include "sim/Simulator.h"
#include "sim/SimulatorBuilder.h"
#include "util/FileSys.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace stride {

using namespace sim_event;
using namespace util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace std;

SimRunner::SimRunner()
    : m_is_running(false), m_operational(false), m_output_prefix(""), m_pt_config(), m_sim(make_shared<Simulator>()),
      m_logger(nullptr)
{
}

bool SimRunner::Setup(const ptree& run_config_pt, shared_ptr<spdlog::logger> logger)
{
        // -----------------------------------------------------------------------------------------
        // Intro.
        // -----------------------------------------------------------------------------------------
        bool status     = true;
        m_pt_config     = run_config_pt;
        m_logger        = std::move(logger);
        m_output_prefix = m_pt_config.get<string>("run.output_prefix");

        // -----------------------------------------------------------------------------------------
        // Create logger for use by the simulator during time step computations.
        // Transmissions: [TRANSMISSION] <infecterID> <infectedID> <contactpoolID> <day>
        // Contacts: [CNT] <person1ID> <person1AGE> <person2AGE> <at_home> <at_work> <at_school> <at_other>
        // -----------------------------------------------------------------------------------------
        spdlog::set_async_mode(1048576);
        auto log_path = FileSys::BuildPath(m_output_prefix, "contact_log.txt");
        /*
        boost::filesystem::path log_path = m_output_prefix;
        if (FileSys::IsDirectoryString(m_output_prefix)) {
                log_path /= "contact_log.txt";
        } else {
                log_path += "_contact_log.txt";
        }*/
        auto file_logger = spdlog::rotating_logger_mt("contact_logger", log_path.c_str(),
                                                      numeric_limits<size_t>::max(), numeric_limits<size_t>::max());
        file_logger->set_pattern("%v"); // Remove meta data from log => time-stamp of logging

        // ------------------------------------------------------------------------------
        // Create the simulator.
        //------------------------------------------------------------------------------
        const auto track_index_case = m_pt_config.get<bool>("run.track_index_case");
        const auto num_threads      = m_pt_config.get<unsigned int>("run.num_threads");
        m_logger->info("Building the simulator.");
        m_sim = SimulatorBuilder::Build(m_pt_config, num_threads, track_index_case);
        m_logger->info("Done building the simulator.");

        // -----------------------------------------------------------------------------------------
        // Check the simulator.
        // -----------------------------------------------------------------------------------------
        if (m_sim->IsOperational()) {
                m_logger->info("Done checking the simulator.");
        } else {
                m_logger->critical("Invalid configuration => terminate without output");
                status = false;
        }
        return status;
}

void SimRunner::Run()
{
        // -----------------------------------------------------------------------------------------
        // Run the simulator.
        // -----------------------------------------------------------------------------------------
        const auto num_days = m_pt_config.get<unsigned int>("run.num_days");
        Notify({shared_from_this(), Id::AtStart});
        for (unsigned int i = 0; i < num_days; i++) {
                m_sim->TimeStep();
                Notify({shared_from_this(), Id::Stepped});
        }
        Notify({shared_from_this(), Id::Finished});
}

} // namespace stride