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

#include <QObject>

namespace stride {
namespace datavis {

class Controller : public QObject
{
	Q_OBJECT

public:
	explicit Controller(QObject* parent = nullptr) : QObject(parent)
	{}

	// prev
	// next
	// readfile
	// etc.
};

} // namespace datavis
} // namespace stride
