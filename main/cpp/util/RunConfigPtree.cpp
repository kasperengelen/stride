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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */
/**
 * @file
 * Implementation of ptree utils.
 */

#include "RunConfigPtree.h"

using namespace boost::property_tree;
using namespace std;

namespace stride {
namespace util {

const ptree& RunConfigPtree::CreateTestsBasic1()
{
        static ptree config_pt;
        if (config_pt.empty()) {

                config_pt.put("run.age_contact_matrix_file", "contact_matrix_flanders_subpop.xml");
                config_pt.put("run.behaviour_policy", "NoBehaviour");
                config_pt.put("run.belief_policy.name", "NoBelief");
                config_pt.put("run.contact_log_level", "None");
                config_pt.put("run.contact_output_file", "false");
                config_pt.put("run.disease_config_file", "disease_influenza.xml");
                config_pt.put("run.global_information_policy", "NoGlobalInformation");
                config_pt.put("run.holidays_file", "holidays_none.json");
                config_pt.put("run.immunity_link_probability", 0);
                config_pt.put("run.immunity_profile", "Random");
                config_pt.put("run.immunity_rate", 0.0);
                config_pt.put("run.local_information_policy", "NoLocalInformation");
                config_pt.put("run.num_days", 30U);
                config_pt.put("run.num_participants_survey", 10);
                config_pt.put("run.num_threads", 1);
                config_pt.put("run.output_prefix", "BatchRuns");
                config_pt.put("run.population_file", "pop_flanders600.csv");
                config_pt.put("run.rng_seed", 2015);
                config_pt.put("run.r0", 3.0);
                config_pt.put("run.seeding_rate", 0.0009);
                config_pt.put("run.seeding_age_min", 1);
                config_pt.put("run.seeding_age_max", 99);
                config_pt.put("run.start_date", "2017-01-01");
                config_pt.put("run.stride_log_level", "info");
                config_pt.put("run.track_index_case", false);
                config_pt.put("run.use_install_dirs", true);
                config_pt.put("run.vaccine_profile", "None");
        }
        return config_pt;
}

const ptree& RunConfigPtree::CreateTestsBasic2()
{
        static ptree config_pt;
        if (config_pt.empty()) {
                config_pt.put("run.age_contact_matrix_file", "contact_matrix_flanders_subpop.xml");
                config_pt.put("run.behaviour_policy", "NoBehaviour");
                config_pt.put("run.belief_policy.name", "NoBelief");
                config_pt.put("run.contact_log_level", "Transmissions");
                config_pt.put("run.contact_output_file", "false");
                config_pt.put("run.disease_config_file", "disease_measles.xml");
                config_pt.put("run.global_information_policy", "NoGlobalInformation");
                config_pt.put("run.holidays_file", "holidays_none.json");
                config_pt.put("run.immunity_rate", 0.8);
                config_pt.put("run.immunity_profile", "None");
                config_pt.put("run.local_information_policy", "NoLocalInformation");
                config_pt.put("run.num_days", 50U);
                config_pt.put("run.num_participants_survey", 10);
                config_pt.put("run.num_threads", 1);
                config_pt.put("run.output_prefix", "BatchRuns");
                config_pt.put("run.population_file", "pop_flanders600.csv");
                config_pt.put("run.rng_seed", 1);
                config_pt.put("run.seeding_age_max", 99);
                config_pt.put("run.seeding_age_min", 1);
                config_pt.put("run.seeding_rate", 0.002);
                config_pt.put("run.start_date", "2017-01-01");
                config_pt.put("run.stride_log_level", "info");
                config_pt.put("run.track_index_case", false);
                config_pt.put("run.use_install_dirs", true);
                config_pt.put("run.vaccine_link_probability", 0);
                config_pt.put("run.vaccine_profile", "Random");
                config_pt.put("run.vaccine_rate", 0.8);
        }
        return config_pt;
}

const ptree& RunConfigPtree::CreateParMeasles()
{
        static ptree config_pt;
        if (config_pt.empty()) {
                config_pt.put("run.age_contact_matrix_file", "contact_matrix_flanders_subpop.xml");
                config_pt.put("run.behaviour_policy", "NoBehaviour");
                config_pt.put("run.belief_policy.name", "NoBelief");
                config_pt.put("run.contact_log_level", "All");
                config_pt.put("run.contact_output_file", "false");
                config_pt.put("run.disease_config_file", "disease_measles.xml");
                config_pt.put("run.global_information_policy", "NoGlobalInformation");
                config_pt.put("run.holidays_file", "holidays_none.json");
                config_pt.put("run.immunity_profile", "None");
                config_pt.put("run.immunity_rate", 0.01);
                config_pt.put("run.local_information_policy", "NoLocalInformation");
                config_pt.put("run.num_days", 30U);
                config_pt.put("run.num_participants_survey", 10);
                config_pt.put("run.num_threads", 1U);
                config_pt.put("run.output_prefix", "bench");
                config_pt.put("run.population_file", "pop_flanders1600.csv");
                config_pt.put("run.rng_seed", 1U);
                config_pt.put("run.r0", 15U);
                config_pt.put("run.seeding_age_max", 99);
                config_pt.put("run.seeding_age_min", 1);
                config_pt.put("run.seeding_rate", 0.02);
                config_pt.put("run.start_date", "2017-01-01");
                config_pt.put("run.stride_log_level", "info");
                config_pt.put("run.track_index_case", false);
                config_pt.put("run.use_install_dirs", true);
                config_pt.put("run.vaccine_link_probability", 0);
                config_pt.put("run.vaccine_profile", "Random");
                config_pt.put("run.vaccine_rate", 0.0);
        }
        return config_pt;
}

} // namespace util
} // namespace stride
