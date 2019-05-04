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

class LocalityView {
public:
	/**
	 * Constructor that creates a view for a Locality object.
	 */
	explicit LocalityView(const Locality& loc);

	/**
	 * Retrieve a QVariantMap that contains information
	 * to display the Locality in QT.
	 */
	const QVariantMap& GetQVariantMap() const {
		return m_locality_data;
	}

private:

	/// Create a QVariantMap that contains information about the specified PopSection.
	const QVariantMap CreatePopSectionView(const PopSection& popSection);

	/// Stores information about the locality.
	QVariantMap m_locality_data;
};

}
}

