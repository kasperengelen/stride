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
 *  Copyright 2018, Jan Broeckhove and Bistromatics group.
 */

#include "GeoGridWriterFactory.h"

#include "GeoGridHDF5Writer.h"
#include "GeoGridJSONWriter.h"
#include "GeoGridProtoWriter.h"
#include "GeoGridWriter.h"
#include "util/Exception.h"

#include <iostream>
#include <memory>

using namespace std;

namespace geopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::CreateGeoGridWriter(const filesys::path& path,
                                                                         shared_ptr<ostream>  outputStream)
{
        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>(move(outputStream));
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoWriter>(move(outputStream));
        } else if (path.extension().string() == ".h5") {
                return std::make_shared<GeoGridHDF5Writer>(path.string());
        } else {
                throw stride::util::Exception("GeoGridWriterFactory::CreateWriter> Unsupported file extension: " +
                                              path.extension().string());
        }
}

} // namespace geopop
