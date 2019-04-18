#include "GeoGridCompare.h"

#include "pop/Population.h"

#include <gtest/gtest.h>

using namespace std;
using namespace geopop;
using namespace stride;
using namespace stride::ContactType;
using namespace util;

void GeoGridCompare::CompareGeoGrid(const GeoGrid& geoGrid1, const GeoGrid& geoGrid2)
{
        ASSERT_EQ(geoGrid1.size(), geoGrid2.size());
        for (unsigned int idx = 0; idx < geoGrid1.size(); idx++) {
                auto location1 = geoGrid1[idx];
                auto location2 = geoGrid2[idx];
                CompareLocation(*location1, *location2);
        }
        ComparePersons();

        m_persons_found1.clear();
        m_persons_found2.clear();
}

void GeoGridCompare::CompareLocation(Location& location1, Location& location2)
{
        EXPECT_EQ(location1.GetName(), location2.GetName());
        EXPECT_EQ(location1.GetProvince(), location2.GetProvince());
        EXPECT_EQ(location1.GetPopCount(), location2.GetPopCount());
        CompareCoordinate(location1.GetCoordinate(), location2.GetCoordinate());

        for (auto id : IdList) {
                CompareContactPools(location1.RefPools(id), location2.RefPools(id));
        }

        ASSERT_EQ(location1.CRefOutgoingCommutes().size(), location2.CRefOutgoingCommutes().size());
        for (unsigned long idx = 0; idx < location1.CRefOutgoingCommutes().size(); idx++) {
                auto commute_pair1 = location1.CRefOutgoingCommutes()[idx];
                auto commute_pair2 = location2.CRefOutgoingCommutes()[idx];
                EXPECT_EQ(commute_pair1.first->GetID(), commute_pair2.first->GetID());
                EXPECT_EQ(commute_pair2.second, commute_pair2.second);
        }
}

void GeoGridCompare::CompareCoordinate(const Coordinate& coordinate1, const Coordinate& coordinate2)
{
        using boost::geometry::get;
        EXPECT_EQ(get<0>(coordinate1), get<0>(coordinate2));
        EXPECT_EQ(get<1>(coordinate1), get<1>(coordinate2));
}

void GeoGridCompare::CompareContactPools(SegmentedVector<ContactPool*>& contactPools1, SegmentedVector<ContactPool*>& contactPools2)
{
        ASSERT_EQ(contactPools1.size(), contactPools2.size());

        auto sorter = [](ContactPool* p1, ContactPool* p2){ return p1->GetId() < p2->GetId(); };
        sort(contactPools1.begin(), contactPools1.end(), sorter);
        sort(contactPools1.begin(), contactPools1.end(), sorter);

        for (unsigned long idx = 0; idx < contactPools1.size(); idx++) {
                auto contactPool1 = contactPools1[idx];
                auto contactPool2 = contactPools2[idx];
                EXPECT_EQ(contactPool1->GetId(), contactPool2->GetId());
                CompareContactPool(contactPool1, contactPool2);
        }
}

void GeoGridCompare::CompareContactPool(ContactPool* contactPool1, ContactPool* contactPool2)
{
        ASSERT_EQ(contactPool1->size(), contactPool2->size());
        for (unsigned long idx = 0; idx < contactPool1->size(); idx++) {
                Person* person1 = contactPool1->begin()[idx];
                Person* person2 = contactPool2->begin()[idx];
                EXPECT_EQ(person1->GetId(), person2->GetId());
                m_persons_found1[person1->GetId()] = person1;
                m_persons_found2[person2->GetId()] = person2;
        }
}

void GeoGridCompare::ComparePersons()
{
        ASSERT_EQ(m_persons_found1.size(), m_persons_found2.size());
        for (auto& it : m_persons_found1) {
                auto person1 = it.second;
                auto person2 = m_persons_found2[it.first];
                EXPECT_EQ(person1->GetAge(), person2->GetAge());
                EXPECT_EQ(person1->GetPoolId(Id::College), person2->GetPoolId(Id::College));
                EXPECT_EQ(person1->GetPoolId(Id::K12School), person2->GetPoolId(Id::K12School));
                EXPECT_EQ(person1->GetPoolId(Id::Household), person2->GetPoolId(Id::Household));
                EXPECT_EQ(person1->GetPoolId(Id::Workplace), person2->GetPoolId(Id::Workplace));
                EXPECT_EQ(person1->GetPoolId(Id::Daycare), person2->GetPoolId(Id::Daycare));
                EXPECT_EQ(person1->GetPoolId(Id::PreSchool), person2->GetPoolId(Id::PreSchool));
                EXPECT_EQ(person1->GetPoolId(Id::PrimaryCommunity), person2->GetPoolId(Id::PrimaryCommunity));
                EXPECT_EQ(person1->GetPoolId(Id::SecondaryCommunity), person2->GetPoolId(Id::SecondaryCommunity));
        }
}
