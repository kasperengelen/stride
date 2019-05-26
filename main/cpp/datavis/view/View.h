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
#include <QVariant>

namespace stride {
namespace datavis {

/**
 * @class View
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
         *
         * @param parent Pointer to QObject parent object.
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

        /**
         * Command the View to display the specified population data in the sidebar.
         *
         * @param popData A PopData object that contains the information that is to be displayed.
         */
        void DisplayPopDataInSidebar(const PopData& popData);

public:
        /**
         * Retrieve simulation data from the model.
         *
         * The format of the QVariant is an array of locations. Each location is
         * a QVariantMap.
         *
         * See PopDataView class for the format of the map. Note that in addition
         * to the map keys specified in the PopDataView class, the returned map also has
         * the following extra keys:
         * 	- name: a QString that contains the name of the location.
         * 	- lon: a double that specifies the longitude of the location.
         * 	- lat: a double that specifies the latitude of the location.
         */
        const QVariant GetEpiData() const;

signals:
		/**
		 * Indicates that there is data ready to be displayed in the sidebar.
		 *
		 * The specified QVariantMap contains information about a population. See PopDataView
		 * class for the format of the map.
		 *
		 * @param sidebarData The data that is to be sent to the QML side.
		 */
		void sidebarDataAvailble(const QVariantMap& sidebarData);

private:
        ///> Pointer to model object.
        Model* m_model_ptr;
};

} // namespace datavis
} // namespace stride
