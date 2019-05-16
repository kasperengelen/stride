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
 * Header file for the View class.
 */

#pragma once

#include "datavis/model/Model.h"

#include <QObject>
#include <QWidget>
#include <QVariant>

namespace stride {
namespace datavis {

/**
 * Class that represent the view component of the MVC structure.
 * This class can be accessed by the Qt5 interface to retrieve data to
 * visualise the simulation data.
 */
class View : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QVariant epi_data READ GetEpiData)
public:
        /**
         * Constructor.
         */
        explicit View(QObject* parent = nullptr) : QObject(parent), m_model_ptr(nullptr) {}

        /**
         * Deleted copy CTOR.
         */
        View(const View&) = delete;

        /**
         * Deleted assignment operator.
         */
        View& operator=(const View&) = delete;

        /**
         * Set the pointer to the Model.
         */
        void SetModelPointer(Model* modelPtr) { m_model_ptr = modelPtr; }

public:
        /**
         * Retrieve simulation data from the model.
         */
        const QVariant GetEpiData() const;

private:
        ///> Pointer to model object.
        Model* m_model_ptr;
};

} // namespace datavis
} // namespace stride
