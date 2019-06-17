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
 * Implementation file for the ProtobufReader class.
 */

#include "ProtobufEpiReader.h"

// NOTE: this is defined in CMakeLists.txt under "set_source_files_properties".
// DO NOT CHANGE!
#include "epiformat.pb.h"
#include "visualiser/readers/EpiReaderException.h"

namespace stride {
namespace visualiser {

using geopop::VisLocation;
using geopop::PoolStats;
using geopop::PopStats;

const VisLocation ParseLocation(const proto::Location& protoLocation);

void ProtobufEpiReader::ReadIntoModel(stride::visualiser::Model& model) const
{
    try {
        // open epi file
        proto::EpiFile epi_file;
        if(!epi_file.ParseFromIstream(this->GetInStream().get()))
        {
            throw EpiReaderException("Error while parsing protobuf file.");
        }

        std::vector<std::vector<VisLocation>> timesteps;

        // iterate over timesteps
        for(const auto& proto_timestep : epi_file.timesteps())
        {
            std::vector<VisLocation> locations{};

            for(const auto& proto_location : proto_timestep.locations())
            {
                const VisLocation loc = ParseLocation(proto_location);

                locations.push_back(loc);
            }

            timesteps.push_back(locations);

        }

        model.SetTimesteps(timesteps);

    } catch (const std::exception& e) {
        throw EpiReaderException(e.what());
    }
}

const geopop::Coordinate ParseCoordinate(const proto::Coordinate& protoCoord)
{
    return {protoCoord.longitude(), protoCoord.latitude()};
}

void ReadPoolIntoPopStats(PopStats& popStats, const proto::PopSection& protoPool, const ContactType::Id& poolType)
{
    PoolStats poolstats;

    poolstats.population  = protoPool.population();
    poolstats.immune      = protoPool.immune();
    poolstats.infected    = protoPool.infected();
    poolstats.infectious  = protoPool.infectious();
    poolstats.recovered   = protoPool.recovered();
    poolstats.susceptible = protoPool.susceptible();
    poolstats.symptomatic = protoPool.symptomatic();

    popStats.SetPool(poolType, poolstats);
}

const VisLocation ParseLocation(const proto::Location& protoLocation)
{
    // name
    const std::string& name = protoLocation.name();

    // coordinate
    const geopop::Coordinate coord = ParseCoordinate(protoLocation.coord());

    // population
    PopStats popstats;

    ReadPoolIntoPopStats(popstats, protoLocation.household(), ContactType::Id::Household);
    ReadPoolIntoPopStats(popstats, protoLocation.k12school(), ContactType::Id::K12School);
    ReadPoolIntoPopStats(popstats, protoLocation.college(),   ContactType::Id::College);
    ReadPoolIntoPopStats(popstats, protoLocation.workplace(), ContactType::Id::Workplace);
    ReadPoolIntoPopStats(popstats, protoLocation.primcom(),   ContactType::Id::PrimaryCommunity);
    ReadPoolIntoPopStats(popstats, protoLocation.seccom(),    ContactType::Id::SecondaryCommunity);
    ReadPoolIntoPopStats(popstats, protoLocation.daycare(),   ContactType::Id::Daycare);
    ReadPoolIntoPopStats(popstats, protoLocation.preschool(), ContactType::Id::PreSchool);

    return VisLocation{coord, name, popstats};
}

} // namespace visualiser
} // namespace stride
