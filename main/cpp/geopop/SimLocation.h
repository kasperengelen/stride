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
 *  Copyright 2018, 2019, Jan Broeckhove and Bistromatics group.
 */

/**
 *
 */

#pragma once

#include "contact/ContactType.h"
#include "contact/IdSubscriptArray.h"
#include "geopop/Coordinate.h"
#include "geopop/LocationBase.h"
#include "util/SegmentedVector.h"

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace stride {
class ContactPool;
}

namespace geopop {

/**
 * Class that represents a location for use in the simulations. This class will be used
 * by GeoGrid.
 */
class SimLocation : public LocationBase
{
public:
        /// Parametrized constructor with population count.
        SimLocation(unsigned int id, unsigned int province, Coordinate coordinate = Coordinate{0.0, 0.0},
                    std::string name = "", unsigned int popCount = 0U);

        /// Perform a full comparison with the other location.
        bool operator==(const SimLocation& other) const;

        /// Gets ID of this Location.
        unsigned int GetID() const { return m_id; }

        /// Gets the province.
        unsigned int GetProvince() const { return m_province; }

        /// Adds a Location and a proportion to the incoming commute vector.
        /// I.e. fraction of commuting population at otherLocation commuting to this Location.
        void AddIncomingCommute(std::shared_ptr<SimLocation> otherLocation, double fraction);

        /// Adds a Location and a fraction to the outgoing commute vector.
        /// I.e. fraction of commuting population at this Location commuting to otherLocation.
        void AddOutgoingCommute(std::shared_ptr<SimLocation> otherLocation, double fraction);

        /// Calculates number of incomming commuters, given the fraction of the population that commutes.
        int GetIncomingCommuteCount(double fractionCommuters) const;

        /// Gets the number of people infected in the contactpools at this location.
        unsigned int GetInfectedCount() const;

        /// Calculates number of outgoing commuters, given the fraction of the population that commutes.
        unsigned int GetOutgoingCommuteCount(double fractionCommuters) const;

        /// Gets the absolute population.
        unsigned int GetPopCount() const { return m_pop_count; }

        /// Get Location's population fraction (of the total populaion count).
        double GetPopFraction() const;

        /// Set Location's population count using its population fraction and the total population count.
        void SetPopCount(unsigned int totalPopCount);

        /// Set Location's population fraction (of the total populaion count).
        void SetPopFraction(double relativePopulation);

public:
        /// Access through const reference to ContactPools of type 'id'.
        /// \param id   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        const stride::util::SegmentedVector<stride::ContactPool*>& CRefPools(stride::ContactType::Id id) const
        {
                return m_pool_index[id];
        }

        /// Templated version of @CRefPools for use when the type id is fixed
        /// \tparam T   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        template <stride::ContactType::Id T>
        const stride::util::SegmentedVector<stride::ContactPool*>& CRefPools() const
        {
                return m_pool_index[T];
        }

        /// Access through reference to ContactPools of type 'id'.
        /// \param id   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        stride::util::SegmentedVector<stride::ContactPool*>& RefPools(stride::ContactType::Id id)
        {
                return m_pool_index[id];
        }

        /// Templated version of @RefPools for use when the type id is fixed
        /// \tparam T   ContactType::Id of pools container you want to access.
        /// \return     The requested reference.
        template <stride::ContactType::Id T>
        stride::util::SegmentedVector<stride::ContactPool*>& RefPools()
        {
                return m_pool_index[T];
        }

        /// Register a ContactPool pointer in this Location's pool system.
        /// Prior to this the pool should have been created in Population's pool system.
        void RegisterPool(stride::ContactPool* p, stride::ContactType::Id typeId)
        {
                m_pool_index[typeId].emplace_back(p);
        }

        /// Templated version of @RegisterPool
        template <stride::ContactType::Id T>
        void RegisterPool(stride::ContactPool* p)
        {
                m_pool_index[T].emplace_back(p);
        }

public:
        /// References incoming commute Locations + fraction of commutes to that Location.
        const std::vector<std::pair<SimLocation*, double>>& CRefIncomingCommutes() const { return m_inCommutes; }

        /// References outgoing commute Locations + fraction of commutes to that Location.
        const std::vector<std::pair<SimLocation*, double>>& CRefOutgoingCommutes() const { return m_outCommutes; }

private:
        unsigned int m_id = 0U;      ///< Id.
        unsigned int m_province;     ///< Province id.
        unsigned int m_pop_count;    ///< Population count (number of individuals) at this Location.
        double       m_pop_fraction; ///< Fraction of whole population at this Location.

        /// Incomming commutes stored as pair of Location and fraction of population at that Location.
        std::vector<std::pair<SimLocation*, double>> m_inCommutes;

        ///< Outgoing commutes stored as pair of Location and fraction of population to this this Location.
        std::vector<std::pair<SimLocation*, double>> m_outCommutes;

        ///< The system holding pointers to the contactpools (for each type id) at this Location.
        stride::ContactType::IdSubscriptArray<stride::util::SegmentedVector<stride::ContactPool*>> m_pool_index;
};

} // namespace geopop
