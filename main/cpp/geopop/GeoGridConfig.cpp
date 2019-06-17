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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridConfig.h"

#include "contact/AgeBrackets.h"
#include "geopop/io/HouseholdReader.h"
#include "geopop/io/ReaderFactory.h"
#include "util/StringUtils.h"
#include "util/FileSys.h"

#include <boost/property_tree/ptree.hpp>
#include <cmath>
#include <iomanip>
#include <numeric>

namespace geopop {

using namespace std;
using namespace stride::util;
using namespace boost::property_tree;
using namespace stride::AgeBrackets;
using namespace stride::ContactType;
using stride::util::intToDottedString;

GeoGridConfig::GeoGridConfig() : param{}, refHH{}, info{}, workplaceSD{} {}

GeoGridConfig::GeoGridConfig(const ptree& configPt) : GeoGridConfig()
{
        const auto pt                      = configPt.get_child("run.geopop_gen");
        param.pop_size                     = pt.get<unsigned int>("population_size");
        param.participation_college        = pt.get<double>("participation_college");
        param.fraction_workplace_commuters = pt.get<double>("fraction_workplace_commuters");
        param.fraction_college_commuters   = pt.get<double>("fraction_college_commuters");
        param.particpation_workplace       = pt.get<double>("particpation_workplace");
        param.participation_daycare        = pt.get<double>("participation_daycare");
        param.participation_preschool      = pt.get<double>("participation_preschool");

        people[Id::K12School]          = pt.get<unsigned int>("people_per_K12School", 500U);
        people[Id::College]            = pt.get<unsigned int>("people_per_College", 3000U);
        people[Id::Workplace]          = pt.get<unsigned int>("people_per_Workplace", 20U);
        people[Id::PrimaryCommunity]   = pt.get<unsigned int>("people_per_PrimaryCommunity", 2000U);
        people[Id::SecondaryCommunity] = pt.get<unsigned int>("people_per_SecondaryCommunity", 2000U);
        people[Id::Daycare]            = pt.get<unsigned int>("people_per_Daycare", 14U);
        people[Id::PreSchool]          = pt.get<unsigned int>("people_per_PreSchool", 90U);

        pools[Id::K12School] = pt.get<unsigned int>("pools_per_K12School", 25U);
        pools[Id::College]   = pt.get<unsigned int>("pools_per_College", 20U);
}

void GeoGridConfig::SetData(const string& householdsFileName)
{
        if (filesys::path(householdsFileName).extension().string() == ".xml") {
                ptree configFile = FileSys::ReadPtreeFile(FileSys::GetDataDir() /= householdsFileName);
                for (const auto& file : configFile.get_child("household_file")) {
                        auto id = configFile.get<unsigned int>("household_file." + file.first + ".<xmlattr>.id");
                        auto householdsReader = ReaderFactory::CreateHouseholdReader(configFile.get<string>("household_file." + file.first));
                        householdsReader->SetReferenceHouseholds(refHH.person_count[id], refHH.ages[id]);
                        refHH.multiHH = true;
                }
        }
        else {
                auto householdsReader = ReaderFactory::CreateHouseholdReader(householdsFileName);
                householdsReader->SetReferenceHouseholds(refHH.person_count[0], refHH.ages[0]);
                refHH.multiHH = false;

        }
        const auto popSize = param.pop_size;

        //----------------------------------------------------------------
        // Determine age makeup of reference houshold population.
        //----------------------------------------------------------------
        const auto ref_p_count = accumulate(refHH.person_count.begin(), refHH.person_count.end(), 0,
                [](const unsigned int prev, const auto& elem) { return prev + elem.second; });
        const auto ref_ages_size = accumulate(refHH.ages.begin(), refHH.ages.end(), 0,
                                            [](const unsigned int prev, const auto& elem) { return prev + elem.second.size(); });
        const auto averageHhSize = static_cast<double>(ref_p_count) / static_cast<double>(ref_ages_size);
        auto ref_preschool = 0U;
        auto ref_daycare   = 0U;
        auto ref_k12school = 0U;
        auto ref_college   = 0U;
        auto ref_workplace = 0U;
        for (const auto& hhAgeProfile : refHH.ages) {
                for (const auto& refAges : hhAgeProfile.second) {
                        for (const auto& age : refAges) {
                                if (Daycare::HasAge(age)) {
                                        ref_daycare++;
                                }
                                if (PreSchool::HasAge(age)) {
                                        ref_preschool++;
                                }
                                if (K12School::HasAge(age)) {
                                        ref_k12school++;
                                }
                                if (College::HasAge(age)) {
                                        ref_college++;
                                }
                                if (Workplace::HasAge(age)) {
                                        ref_workplace++;
                                }
                        }
                }
        }

        //----------------------------------------------------------------
        // Scale up to the generated population size.
        //----------------------------------------------------------------
        const auto fraction_daycare_age   = static_cast<double>(ref_daycare) / static_cast<double>(ref_p_count);
        const auto fraction_preschool_age = static_cast<double>(ref_preschool) / static_cast<double>(ref_p_count);
        const auto fraction_k12school_age = static_cast<double>(ref_k12school) / static_cast<double>(ref_p_count);
        const auto fraction_college_age   = static_cast<double>(ref_college) / static_cast<double>(ref_p_count);
        const auto fraction_workplace_age = static_cast<double>(ref_workplace) / static_cast<double>(ref_p_count);

        const auto age_count_daycare   = static_cast<unsigned int>(floor(popSize * fraction_daycare_age));
        const auto age_count_preschool = static_cast<unsigned int>(floor(popSize * fraction_preschool_age));
        const auto age_count_k12school = static_cast<unsigned int>(floor(popSize * fraction_k12school_age));
        const auto age_count_college   = static_cast<unsigned int>(floor(popSize * fraction_college_age));
        const auto age_count_workplace = static_cast<unsigned int>(floor(popSize * fraction_workplace_age));

        info.popcount_daycare = static_cast<unsigned int>(floor(param.participation_daycare * age_count_daycare));

        info.popcount_preschool = static_cast<unsigned int>(floor(param.participation_preschool * age_count_preschool));

        info.popcount_k12school = age_count_k12school;

        info.popcount_college = static_cast<unsigned int>(floor(param.participation_college * age_count_college));

        info.popcount_workplace = static_cast<unsigned int>(
            floor(param.particpation_workplace * (age_count_workplace - info.popcount_college)));

        info.count_households = static_cast<unsigned int>(floor(static_cast<double>(popSize) / averageHhSize));
}

void GeoGridConfig::SetWorkplaceData(const std::string& workplaceFileName)
{
        auto workplaceReader = ReaderFactory::CreateWorkplaceReader(workplaceFileName);
        workplaceReader->SetWorkplaceData(workplaceSD.ratios, workplaceSD.sizes);
}

ostream& operator<<(ostream& out, const GeoGridConfig& config)
{
        const int w = 53;
        out.setf(std::ios_base::left, std::ios_base::adjustfield);
        out << "Input:" << endl;
        out << setw(w) << "Fraction college commuters:" << config.param.fraction_college_commuters << "\n";
        out << setw(w) << "Fraction workplace commuters:" << config.param.fraction_workplace_commuters << "\n";
        out << setw(w) << "Participation fraction of daycare:" << config.param.participation_daycare << "\n";
        out << setw(w) << "Participation fraction of preschool:" << config.param.participation_preschool << "\n";
        out << setw(w) << "Participation fraction of college:" << config.param.participation_college << "\n";
        out << setw(w) << "Participation fraaction of workplace:" << config.param.particpation_workplace << "\n";
        out << setw(w) << "Target population size" << intToDottedString(config.param.pop_size) << "\n"
            << "\n";
        out << "Calculated:"
            << "\n";
        out << setw(w) << "Daycare student count:" << intToDottedString(config.info.popcount_daycare) << "\n";
        out << setw(w) << "PreSchool student count:" << intToDottedString(config.info.popcount_preschool) << "\n";
        out << setw(w) << "K12School student count:" << intToDottedString(config.info.popcount_k12school) << "\n";
        out << setw(w) << "College student count:" << intToDottedString(config.info.popcount_college) << "\n";
        out << setw(w) << "Workplace person count:" << intToDottedString(config.info.popcount_workplace) << "\n";
        out << endl;
        return out;
}

} // namespace geopop
