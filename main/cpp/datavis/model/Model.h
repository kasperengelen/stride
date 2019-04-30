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

#include <QObject>

#include <vector>

namespace stride {
namespace datavis {

/**
 * Class that represents the model of the visualiser. This contains all the stored simulation data.
 */
class Model : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 */
	explicit Model(QObject* parent = nullptr) : QObject(parent), m_timesteps(), m_current_timestep(0)
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

	/**
	 * Retrieve the current timestep.
	 */
	const Timestep& GetCurrentTimestepData();

	/**
	 * Determine whether or not the model has a timestep that comes after the current one.
	 */
	bool HasNextTimestep() const;

	/**
     * Determine whether or not the model has a timestep that comes before the current one.
     */
	bool HasPrevTimestep() const;

	/**
	 * Switch to the next timestep.
	 */
	void NextTimestep();

	/**
	 * Switch to the previous timestep.
	 */
	void PrevTimestep();

	/**
	 * Switch to the first timestep.
	 */
	void FirstTimestep();

	/**
	 * Switch to the last timestep.
	 */
	void LastTimestep();

private:
	/// Contains the currently stored simulation timesteps.
	std::vector<Timestep> m_timesteps;

	/// Contains the index of the current timestep.
	std::size_t m_current_timestep;
};


} // namespace datavis
} // namespace stride
