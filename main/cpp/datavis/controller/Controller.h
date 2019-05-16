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

#include "datavis/model/Model.h"
#include "datavis/view/View.h"

#include <QObject>

namespace stride {
namespace datavis {

/**
 * Class that represents the controller component of the data visualiser MVC structure.
 * This class will take commands from the Qt5 interface and relay them to the Model.
 */
class Controller : public QObject
{
        Q_OBJECT

public:
        explicit Controller(QObject* parent = nullptr) : QObject(parent), m_model_ptr(nullptr) {}

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

public:
        /**
         * Open dialog to open file.
         */
        Q_INVOKABLE void OpenFile();

signals:
        /**
         * Indicates that a successful file read has occurred.
         */
        void fileReadSuccessful();

private:
        ///> Pointer to model object.
        Model* m_model_ptr;
};

} // namespace datavis
} // namespace stride
