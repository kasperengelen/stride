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

#include "PopDataView.h"

namespace stride {
namespace visualiser {

PopDataView::PopDataView(const PopData& popData) : m_pop_data{}
{
        // TODO find a way to use this!
        // m_pop_data.insert("name", QString::fromStdString(loc.GetName()));
        // m_pop_data.insert("lon", loc.GetCoordinate().get<0>());
        // m_pop_data.insert("lat", loc.GetCoordinate().get<1>());

        m_pop_data.insert("total", this->CreatePopSectionView(popData.total));
        m_pop_data.insert("household", this->CreatePopSectionView(popData.household));
        m_pop_data.insert("k12school", this->CreatePopSectionView(popData.k12school));
        m_pop_data.insert("college", this->CreatePopSectionView(popData.college));
        m_pop_data.insert("workplace", this->CreatePopSectionView(popData.workplace));
        m_pop_data.insert("primCom", this->CreatePopSectionView(popData.primCom));
        m_pop_data.insert("secCom", this->CreatePopSectionView(popData.secCom));
        m_pop_data.insert("daycare", this->CreatePopSectionView(popData.daycare));
        m_pop_data.insert("preschool", this->CreatePopSectionView(popData.preschool));
}

const QVariantMap PopDataView::CreatePopSectionView(const PopSection& popSection)
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

} // namespace visualiser
} // namespace stride
