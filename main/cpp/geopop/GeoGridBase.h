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
 * Header file for the GeoGridBase class.
 */

#pragma once

#include "geopop/Location.h"
#include "geopop/geo/GeoAggregator.h"
#include "geopop/geo/GeoGridKdTree.h"

#include <vector>
#include <set>
#include <memory>

namespace geopop {

template <typename Policy, typename... F>
class GeoAggregator;

/**
 * @brief
 * Class that serves as a generalized base class for GeoGrid.
 */
template <typename LocationType>
class GeoGridBase {
public:
    /// Default constructor.
    GeoGridBase();

    /// No copy constructor.
    GeoGridBase(const GeoGridBase&) = delete;

    /// No copy assignment.
    GeoGridBase operator=(const GeoGridBase&) = delete;

    /// Adds a location to this GeoGrid.
    void AddLocation(std::shared_ptr<LocationType> location);

    /// Disables the addLocation method and builds the kdtree.
    void Finalize();

    /// Gets a Location by Id and check if the Id exists.
    std::shared_ptr<LocationType> GetById(unsigned int id) const { return m_locations[m_id_to_index.at(id)]; }

    /// Search for locations in \p radius (in km) around \p start.
    std::vector<const LocationType*> LocationsInRadius(const LocationType& start, double radius) const;

    /**
     * Gets the locations in a rectangle determined by the two coordinates (long1, lat1) and (long2, lat2).
     * The coordinates must be positioned on the diagonal, i.e:
     *
     *  p1 -----+     +-------p1
     *  |       |     |       |
     *  |       |  or |       |
     *  |       |     |       |
     *  +-------p2    p2------+
     */
    std::set<const LocationType*> LocationsInBox(double long1, double lat1, double long2, double lat2) const;

    /// Gets the location in a rectangle defined by the two Locations.
    std::set<const LocationType*> LocationsInBox(LocationType* loc1, LocationType* loc2) const;

public:
    /// Build a GeoAggregator with a predefined functor and given args for the Policy.
    template <typename Policy, typename F>
    GeoAggregator<Policy, F> BuildAggregator(F functor, typename Policy::Args&& args) const;

    /// Build a GeoAggregator that gets its functor when calling, with given args for the Policy.
    template <typename Policy>
    GeoAggregator<Policy> BuildAggregator(typename Policy::Args&& args) const;

public:

    using iterator       = typename std::vector<std::shared_ptr<LocationType>>::iterator;
    using const_iterator = typename std::vector<std::shared_ptr<LocationType>>::const_iterator;

    /// Iterator to first Location.
    iterator begin() { return m_locations.begin(); }

    /// Iterator to the end of the Location storage.
    iterator end() { return m_locations.end(); }

    const_iterator begin() const { return this->cbegin(); }

    const_iterator end() const   { return this->cend(); }

    /// Const Iterator to first Location.
    const_iterator cbegin() const { return m_locations.cbegin(); }

    /// Const iterator to the end of the Location storage.
    const_iterator cend() const { return m_locations.cend(); }

    /// Gets a Location by index, doesn't performs a range check.
    std::shared_ptr<LocationType>& operator[](size_t index) { return m_locations[index]; }

    /// Gets a Location by index, doesn't performs a range check.
    const std::shared_ptr<LocationType>& operator[](size_t index) const { return m_locations[index]; }

    /// Gets current size of Location storage.
    size_t size() const { return m_locations.size(); }

private:
    ///< Checks whether the GeoGrid is finalized i.e. certain operations can(not) be used.
    void CheckFinalized(const std::string& functionName) const;


private:
    ///< Container for Locations in GeoGrid.
    std::vector<std::shared_ptr<LocationType>> m_locations;

    ///< Associative container maps Location Id to index in m_locations.
    std::unordered_map<unsigned int, unsigned int> m_id_to_index;

    ///< Is the GeoGrid finalized (ready for use) yet?
    bool m_finalized;

    ///< Internal KdTree for quick spatial lookup.
    GeoGridKdTree m_tree;
};

template <typename LocationType>
GeoGridBase<LocationType>::GeoGridBase()
        : m_locations(), m_id_to_index(), m_finalized(false), m_tree()
{}

template <typename LocationType>
void GeoGridBase<LocationType>::AddLocation(std::shared_ptr<LocationType> location)
{
    if (m_finalized) {
        throw std::runtime_error("Calling addLocation while GeoGrid is finalized not supported!");
    }
    m_locations.emplace_back(location);
    m_id_to_index[location->GetID()] = static_cast<unsigned int>(m_locations.size() - 1);
}

template <typename LocationType>
void GeoGridBase<LocationType>::CheckFinalized(const std::string& functionName) const
{
    if (!m_finalized) {
        throw std::runtime_error("Calling \"" + functionName + "\" with GeoGrid not finalized not supported!");
    }
}

template <typename LocationType>
void GeoGridBase<LocationType>::Finalize()
{
    std::vector<geogrid_detail::KdTree2DPoint> points;
    for (const auto& loc : m_locations) {
        points.emplace_back(geogrid_detail::KdTree2DPoint(loc.get()));
    }
    m_tree      = GeoGridKdTree::Build(points);
    m_finalized = true;
}

template <typename LocationType>
template <typename Policy, typename F>
GeoAggregator<Policy, F> GeoGridBase<LocationType>::BuildAggregator(F functor, typename Policy::Args&& args) const
{
    return GeoAggregator<Policy, F>(m_tree, functor, std::forward<typename Policy::Args>(args));
}

template <typename LocationType>
template <typename Policy>
GeoAggregator<Policy> GeoGridBase<LocationType>::BuildAggregator(typename Policy::Args&& args) const
{
    return GeoAggregator<Policy>(m_tree, std::forward<typename Policy::Args>(args));
}

template <typename LocationType>
std::set<const LocationType*> GeoGridBase<LocationType>::LocationsInBox(double long1, double lat1, double long2, double lat2) const
{
    CheckFinalized(__func__);

    std::set<const LocationType*> result;

    auto agg = BuildAggregator<BoxPolicy>(
            MakeCollector(inserter(result, result.begin())),
            std::make_tuple(std::min(long1, long2), std::min(lat1, lat2), std::max(long1, long2), std::max(lat1, lat2)));
    agg();

    return result;
}

template <typename LocationType>
std::set<const LocationType*> GeoGridBase<LocationType>::LocationsInBox(LocationType* loc1, LocationType* loc2) const
{
    using boost::geometry::get;
    return LocationsInBox(get<0>(loc1->GetCoordinate()), get<1>(loc1->GetCoordinate()),
                          get<0>(loc2->GetCoordinate()), get<1>(loc2->GetCoordinate()));
}

template <typename LocationType>
std::vector<const LocationType*> GeoGridBase<LocationType>::LocationsInRadius(const LocationType& start, double radius) const
{
    CheckFinalized(__func__);

    geogrid_detail::KdTree2DPoint startPt(&start);
    std::vector<const LocationType*>  result;

    auto agg = BuildAggregator<RadiusPolicy>(MakeCollector(back_inserter(result)), std::make_tuple(startPt, radius));
    agg();

    return result;
}

} // namespace geopop
