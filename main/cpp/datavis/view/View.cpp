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
 * Implementation file for the View class.
 */

#include "View.h"

#include "datavis/view/PopDataView.h"

namespace stride {
namespace datavis {

const QVariant View::GetEpiData() const
{
        const auto& timesteps = m_model_ptr->GetEpiData();

        QVariantList retval;

        for (const auto& timestep : timesteps) {
                QVariantList loc_list;

                for (const auto& loc : timestep) {
                        QVariantMap pop_data = PopDataView{loc.GetPopData()}.GetQVariantMap();

                        // add some location info to the map.
                        pop_data.insert("name", QString::fromStdString(loc.GetName()));
						pop_data.insert("lon", loc.GetCoordinate().get<0>());
                        pop_data.insert("lat", loc.GetCoordinate().get<1>());

                        loc_list.push_back(pop_data);
                }

                std::sort(loc_list.begin(), loc_list.end(),
                	[](const QVariant& a, const QVariant& b) -> bool
				{
                	// determine whether or not "a" should go before "b"

                	const QVariantMap& a_total = a.toMap()["total"].toMap();
                	const QVariantMap& b_total = b.toMap()["total"].toMap();

                	return a_total["pop"] > b_total["pop"];

				});

                retval.push_back(loc_list);
        }

        return QVariant::fromValue(retval);
}

void View::DisplayPopDataInSidebar(const PopData& popData)
{
	emit this->sidebarDataAvailble(PopDataView(popData).GetQVariantMap());
}

} // namespace datavis
} // namespace stride
