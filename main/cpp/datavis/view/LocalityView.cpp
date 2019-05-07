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
 * Implementation file for the Locality class.
 */

#include "LocalityView.h"

namespace stride {
namespace datavis {

LocalityView::LocalityView(const Locality& loc) : m_locality_data{}
{
        m_locality_data.insert("name", QString::fromStdString(loc.GetName()));
        m_locality_data.insert("lon", loc.GetCoordinate().get<0>());
        m_locality_data.insert("lat", loc.GetCoordinate().get<1>());

        m_locality_data.insert("total", this->CreatePopSectionView(loc.GetTotalPopData()));
        m_locality_data.insert("household", this->CreatePopSectionView(loc.GetHouseholdPopData()));
        m_locality_data.insert("k12school", this->CreatePopSectionView(loc.GetK12SchoolPopData()));
        m_locality_data.insert("college", this->CreatePopSectionView(loc.GetCollegePopData()));
        m_locality_data.insert("workplace", this->CreatePopSectionView(loc.GetWorkplacePopData()));
        m_locality_data.insert("primCom", this->CreatePopSectionView(loc.GetPrimCommunityPopData()));
        m_locality_data.insert("secCom", this->CreatePopSectionView(loc.GetSecCommunityPopData()));
        m_locality_data.insert("daycare", this->CreatePopSectionView(loc.GetDaycarePopData()));
        m_locality_data.insert("preschool", this->CreatePopSectionView(loc.GetPreSchoolPopData()));
}

const QVariantMap LocalityView::CreatePopSectionView(const PopSection& popSection)
{
        QVariantMap retval{};

        retval.insert("pop", popSection.pop);
        retval.insert("immune", popSection.immune);
        retval.insert("infected", popSection.infected);
        retval.insert("infectious", popSection.infectious);
        retval.insert("recovered", popSection.recovered);
        retval.insert("susceptible", popSection.susceptible);
        retval.insert("symptomatic", popSection.symptomatic);

        return retval;
}

} // namespace datavis
} // namespace stride
