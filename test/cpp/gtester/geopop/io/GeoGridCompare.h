#pragma once

#include "contact/ContactType.h"
#include "geopop/GeoGrid.h"
#include "geopop/Location.h"
#include "pop/Person.h"
#include <map>

class GeoGridCompare
{
public:
        GeoGridCompare() : m_persons_found1(), m_persons_found2(){};

        void CompareGeoGrid(const geopop::GeoGrid& geoGrid1, const geopop::GeoGrid& geoGrid2);

private:
        void CompareLocation(geopop::Location& location1, geopop::Location& location2);

        void CompareCoordinate(const geopop::Coordinate& coordinate1, const geopop::Coordinate& coordinate2);

        void CompareContactPools(stride::util::SegmentedVector<stride::ContactPool*>& contactPools1,
                                 stride::util::SegmentedVector<stride::ContactPool*>& contactPools2);

        void CompareContactPool(stride::ContactPool* contactPool1, stride::ContactPool* contactPool2);

        void ComparePersons();

private:
        std::map<int, stride::Person*> m_persons_found1;
        std::map<int, stride::Person*> m_persons_found2;
};
