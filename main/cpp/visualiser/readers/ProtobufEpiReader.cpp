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

#include "visualiser/readers/ProtobufEpiFormat.pb.h"
#include "visualiser/readers/EpiReaderException.h"

namespace stride {
namespace visualiser {

const Locality ParseLocation(const proto::Location& protoLocation);

void ProtobufEpiReader::ReadIntoModel(stride::visualiser::Model& model) const
{
    try {
        // open epi file
        proto::EpiFile epi_file;
        if(!epi_file.ParseFromIstream(this->GetInStream().get()))
        {
            throw EpiReaderException("Error while parsing protobuf file.");
        }

        std::vector<std::vector<Locality>> timesteps;

        // iterate over timesteps
        for(const auto& proto_timestep : epi_file.timesteps())
        {
            std::vector<Locality> locations{};

            for(const auto& proto_location : proto_timestep.locations())
            {
                const Locality loc = ParseLocation(proto_location);

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

const PopSection ParsePopSection(const proto::PopSection& protoPopSection)
{
    PopSection retval;

    retval.pop         = protoPopSection.population();
    retval.immune      = protoPopSection.immune();
    retval.infected    = protoPopSection.infected();
    retval.infectious  = protoPopSection.infectious();
    retval.recovered   = protoPopSection.recovered();
    retval.susceptible = protoPopSection.susceptible();
    retval.symptomatic = protoPopSection.symptomatic();

    return retval;
}

const Locality ParseLocation(const proto::Location& protoLocation)
{
    // name
    const std::string& name = protoLocation.name();

    // coordinate
    const geopop::Coordinate coord = ParseCoordinate(protoLocation.coord());

    // pools
    const PopSection total      = ParsePopSection(protoLocation.household()); // Note: households contain the total, so we just copy it
    const PopSection household  = ParsePopSection(protoLocation.household());
    const PopSection k12_school = ParsePopSection(protoLocation.k12school());
    const PopSection college    = ParsePopSection(protoLocation.college());
    const PopSection workplace  = ParsePopSection(protoLocation.workplace());
    const PopSection prim_com   = ParsePopSection(protoLocation.primcom());
    const PopSection sec_com    = ParsePopSection(protoLocation.seccom());
    const PopSection daycare    = ParsePopSection(protoLocation.daycare());
    const PopSection preschool  = ParsePopSection(protoLocation.preschool());

    // popdata
    const PopData population = {
            total,
            household,
            k12_school,
            college,
            workplace,
            prim_com,
            sec_com,
            daycare,
            preschool
    };

    return Locality(name, coord, population);
}

} // namespace visualiser
} // namespace stride
