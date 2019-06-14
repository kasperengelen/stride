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

#include "protobuf/epiformat.pb.h"

#include "contact/ContactType.h"
#include "pop/Population.h"
#include "geopop/Location.h"
#include "util/FileSys.h"

namespace stride {
namespace output {

EpiOutputProto::EpiOutputProto(const std::string &output_prefix) : EpiOutputFile(), m_proto_file{} {
    Initialize(output_prefix);
}

void EpiOutputProto::Initialize(const std::string &output_prefix) {
    const auto p = util::FileSys::BuildPath(output_prefix, "EpiOutput.proto");
    m_fstream.open(p.c_str(), std::ios::trunc | std::ios::out);
}

void EpiOutputProto::Update(std::shared_ptr<const Population> population) {

    proto::Timestep timestep;



    const geopop::GeoGrid &geogrid = population->CRefGeoGrid();
    for (const auto& location: geogrid) {
        const LocationPopData popdata{*location};

        proto::Location proto_loc;

        // set name
        proto_loc.set_name(location->GetName());

        // set coordinate
        proto::Coordinate coord;
        coord.set_longitude(location->GetCoordinate().get<0>());
        coord.set_latitude(location->GetCoordinate().get<1>());



        // TODO set pools


        // TODO add location to timestep
    }

    // TODO add timestep to file
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
