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
 * @file
 */

#include "EpiOutputProto.h"

#include "viewers/LocationPopStats.h"

#include "contact/ContactType.h"
#include "pop/Population.h"
#include "geopop/Location.h"
#include "util/FileSys.h"

#include <iostream>

namespace stride {
namespace output {

/**
 * Given a PoolStats object, this will return a pointer to a proto::PopSection object
 * that contains the same information in protobuf format.
 */
proto::PopSection* SerializePopSection(const PoolStats& pool_stats)
{
        proto::PopSection* retval = new proto::PopSection{};

        retval->set_population(pool_stats.population);
        retval->set_immune(pool_stats.immune);
        retval->set_infected(pool_stats.infected);
        retval->set_infectious(pool_stats.infectious);
        retval->set_recovered(pool_stats.recovered);
        retval->set_susceptible(pool_stats.susceptible);
        retval->set_symptomatic(pool_stats.symptomatic);

        return retval;
}

EpiOutputProto::EpiOutputProto(const std::string &output_prefix) : EpiOutputFile(), m_proto_file{} {
        Initialize(output_prefix);
}

void EpiOutputProto::Initialize(const std::string &output_prefix) {
        const auto p = util::FileSys::BuildPath(output_prefix, "EpiOutput.proto");
        m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
}

void EpiOutputProto::Update(std::shared_ptr<const Population> population) {
        proto::Timestep* timestep = m_proto_file.add_timesteps();

        const geopop::GeoGrid &geogrid = population->CRefGeoGrid();
        for (const auto& location: geogrid) {
                // add location to timestep
                proto::Location* proto_loc = timestep->add_locations();

                // set name
                proto_loc->set_name(location->GetName());

                // set coordinate
                // NOTE: no need to delete object, protobuf takes ownership!
                proto::Coordinate* coord = new proto::Coordinate{};
                coord->set_longitude(location->GetCoordinate().get<0>());
                coord->set_latitude(location->GetCoordinate().get<1>());
                proto_loc->set_allocated_coord(coord);

                // determine population stats
                const LocationPopData popdata{*location};

                // set pool stats
                proto_loc->set_allocated_household(SerializePopSection(popdata.GetPool(ContactType::Id::Household)));
                proto_loc->set_allocated_college(  SerializePopSection(popdata.GetPool(ContactType::Id::College)));
                proto_loc->set_allocated_daycare(  SerializePopSection(popdata.GetPool(ContactType::Id::Daycare)));
                proto_loc->set_allocated_k12school(SerializePopSection(popdata.GetPool(ContactType::Id::K12School)));
                proto_loc->set_allocated_preschool(SerializePopSection(popdata.GetPool(ContactType::Id::PreSchool)));
                proto_loc->set_allocated_primcom(  SerializePopSection(popdata.GetPool(ContactType::Id::PrimaryCommunity)));
                proto_loc->set_allocated_seccom(   SerializePopSection(popdata.GetPool(ContactType::Id::SecondaryCommunity)));
                proto_loc->set_allocated_workplace(SerializePopSection(popdata.GetPool(ContactType::Id::Workplace)));
        }
}

void EpiOutputProto::Finish()
{
        if(!m_proto_file.SerializeToOstream(&m_fstream))
        {
                throw std::runtime_error{"Cannot serialize protobuf output to file."};
        }
}

} // namespace output
} // namespace stride
