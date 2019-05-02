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
 * Header file for the Model class.
 */

#pragma once

#include "datavis/model/Timestep.h"

#include <QQmlPropertyMap>
#include <QObject>
#include <QVariantMap>
#include <QVariantList>

#include <vector>

namespace stride {
namespace datavis {

/**
 * Class that represents the model of the visualiser. This contains all the stored simulation data.
 */
class Model : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVariant loc_list READ GetViewList)

public:
	/**
	 * Constructor.
	 */
	explicit Model(QObject* parent = nullptr) : QObject(parent), m_timesteps()
	{}

	/**
	 * Deleted copy CTOR.
	 */
	Model(const Model&) = delete;

	/**
	 * Deleted assignment operator.
	 */
	Model& operator=(const Model&) = delete;

	/**
	 * Add a timestep to the model.
	 */
	void AddTimestep(const Timestep& timestep);

	/**
	 * Remove all the timesteps from the model.
	 */
	void ClearTimesteps();

	const QVariant GetViewList() const
	{
		//return {new LocalityView(this), new LocalityView(this), new LocalityView(this)};

		QVariantList list;

		for(const auto& loc : m_timesteps.at(0).GetLocalities())
		{
			list.push_back(loc.GetView());
		}

		return QVariant::fromValue(list);
	}


private:
	/// Contains the currently stored simulation timesteps.
	std::vector<Timestep> m_timesteps;
};


} // namespace datavis
} // namespace stride
