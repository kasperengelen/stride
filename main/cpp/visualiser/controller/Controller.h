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
 * Header file for the Controller class.
 */

#include "visualiser/model/Model.h"
#include "visualiser/view/View.h"

#include <QObject>
#include <QGeoCoordinate>

namespace stride {
namespace visualiser {

/**
 * @class Controller 
 * Class that represents the controller component of the data visualiser MVC structure.
 * This class will take commands from the Qt5 interface and relay them to the Model.
 */
class Controller : public QObject
{
        Q_OBJECT

public:
		/**
		 * Constructor based on a QObject parent.
		 */
        explicit Controller(QObject* parent = nullptr)
        	: QObject(parent), m_model_ptr(nullptr), m_view_ptr(nullptr)
        {}

        /**
         * Deleted copy CTOR.
         */
        Controller(const Controller&) = delete;

        /**
         * Deleted assignment operator.
         */
        Controller& operator=(const Controller&) = delete;

        /**
         * Set the pointer to the Model.
         */
        void SetModelPointer(Model* modelPtr) { m_model_ptr = modelPtr; }

        /**
         * Set the pointer to the View.
         */
        void SetViewPointer(View* viewPtr) { m_view_ptr = viewPtr; }

public:
        /**
         * Open dialog to open file.
         */
        Q_INVOKABLE void OpenFile();

        /**
         * Open dialog to save file.
         */
        Q_INVOKABLE void SaveFile();

        /**
         * Select locations as they appeared on the selected day
         * in a circular area with the specified radius around
         * the specified coordinate.
         */
        Q_INVOKABLE void SelectRadius(QGeoCoordinate coord, float radius, unsigned int day);

        /**
         * Select locations as they appeared on the selected day
         * in a rectangular area between the two specified coordinates.
         */
        Q_INVOKABLE void SelectRectangular(QGeoCoordinate pointA, QGeoCoordinate pointB, unsigned int day);

signals:
        /**
         * Indicates that a successful file read has occurred.
         */
        void fileReadSuccessful();

        /**
         * Indicates that the map needs to be saved to a image file.
         */
        void saveMapToFile(const QString& filename);

private:
        ///> Pointer to model object.
        Model* m_model_ptr;

        ///> Pointer to view object.
        View* m_view_ptr;
};

} // namespace visualiser
} // namespace stride
