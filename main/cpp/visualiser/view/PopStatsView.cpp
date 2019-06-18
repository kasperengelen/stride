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

#include "PopStatsView.h"

namespace stride {
namespace visualiser {

PopStatsView::PopStatsView(const geopop::PopStats& popStats) : m_pop_data{} {
    this->AddPoolToView("total",     popStats, ContactType::Id::Household);
    this->AddPoolToView("household", popStats, ContactType::Id::Household);
    this->AddPoolToView("k12school", popStats, ContactType::Id::K12School);
    this->AddPoolToView("college",   popStats, ContactType::Id::College);
    this->AddPoolToView("workplace", popStats, ContactType::Id::Workplace);
    this->AddPoolToView("primCom",   popStats, ContactType::Id::PrimaryCommunity);
    this->AddPoolToView("secCom",    popStats, ContactType::Id::SecondaryCommunity);
    this->AddPoolToView("daycare",   popStats, ContactType::Id::Daycare);
    this->AddPoolToView("preschool", popStats, ContactType::Id::PreSchool);
}

void PopStatsView::AddPoolToView(const QString& mapKey, const PopStats& popStats, ContactType::Id poolType)
{
    const geopop::PoolStats& pool_stats = popStats.GetPool(poolType);

    QVariantMap pool_view{};

    pool_view.insert("pop",         pool_stats.population);
    pool_view.insert("immune",      pool_stats.immune);
    pool_view.insert("infected",    pool_stats.infected);
    pool_view.insert("infectious",  pool_stats.infectious);
    pool_view.insert("recovered",   pool_stats.recovered);
    pool_view.insert("susceptible", pool_stats.susceptible);
    pool_view.insert("symptomatic", pool_stats.symptomatic);

    m_pop_data.insert(mapKey, pool_view);
}

} // namespace visualiser
} // namespace stride
