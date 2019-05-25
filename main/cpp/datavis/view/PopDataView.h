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
 * Header file for the LocalityView class.
 */

#pragma once

#include "datavis/model/Locality.h"

#include <QVariant>

namespace stride {
namespace datavis {

/**
 * Class that provides a view for PopData objects. This class
 * can produce a QML-compatible QVariantMap that contains all the
 * information in the PopData object.
 */
class PopDataView
{
public:
        /**
         * Constructor that creates a view for a Locality object.
         */
        explicit PopDataView(const PopData& popData);

        /**
         * Retrieve a QVariantMap that contains information
         * to display the Population in QT.
         *
         * This map has 9 members, each of which represents a subpopulation:
         *   - total: The total population.
         *   - household: The part of the population that is part of a household.
         *   - k12school: The part of the population that is part of a k12 school.
         *   - college: The part of the population that is part of a college.
         *   - workplace: The part of the population that is part of a workplace.
         *   - primCom: The part of the population that is part of a primary community.
         *   - secCom: The part of the population that is part of a secondary community.
         *   - daycare: The part of the population that is part of a daycare.
         *   - preschool: The part of the population that is part of a preschool.
         *
         * Each subpopulation member has the following members:
         *   - pop: the number of individuals in the subpopulation
         *   - immune: The fraction of the population that is immune.
         *   - infected: The fraction of the population that is infected.
         *   - infectious: The fraction of the population that is infectious.
         *   - recovered: The fraction of the population that is recovered.
         *   - susceptible: The fraction of the population that is susceptible.
         *   - symptomatic: The fraction of the population that is symptomatic.
         *
         *  Note that all fractions are between 0 and 1 inclusive.
         */
        const QVariantMap& GetQVariantMap() const { return m_pop_data; }

private:
        /// Create a QVariantMap that contains information about the specified PopSection.
        const QVariantMap CreatePopSectionView(const PopSection& popSection);

        /// Stores information about the population.
        QVariantMap m_pop_data;
};

} // namespace datavis
} // namespace stride
