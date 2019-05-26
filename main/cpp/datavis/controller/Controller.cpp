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
 * Implementation file for the Controller class.
 */

#include "datavis/controller/Controller.h"

#include "datavis/readers/JSONEpiReader.h"
#include "datavis/readers/EpiReaderException.h"

#include <iostream>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

namespace stride {
namespace datavis {

void Controller::OpenFile()
{
        QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());
        QString  filename   = QFileDialog::getOpenFileName(
            parent_ptr, tr("Open epi-data"), "", tr("JSON file (*.json);;HDF5 file (*.h5);;Protobuf file (*.proto)"));

        if (filename.isNull()) {
                return;
        }

        // determine HDF5, Protobuf, JSON
        if (filename.endsWith(".json")) {
                try {
                        // JSON
                        JSONEpiReader reader(filename.toStdString());

                        reader.ReadIntoModel(*m_model_ptr);

                        emit this->fileReadSuccessful();

                        return;
                } catch (const EpiReaderException& e) {
                        const QString err_msg =
                            QString{"An error occurred while processing the specified file.\n"} + QString{e.what()};
                        QMessageBox::critical(parent_ptr, QString{"Error"}, err_msg);
                        return;
                }
        } else {
                // Unknown format
                QMessageBox::critical(parent_ptr, tr("Warning"), tr("Specified file format is not supported."));
                return;
        }

        // TODO HDF5
}

void Controller::SaveFile()
{
		QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());

		QString filename = QFileDialog::getSaveFileName(parent_ptr, tr("Save visualisation"), "",
														tr("PNG file (*.png)"));

		if (filename.isNull()) {
				return;
		}

		if(!filename.endsWith(".png"))
		{
			filename += ".png";
		}

		emit this->saveMapToFile(filename);

		return;
}

void Controller::SelectRadius(QGeoCoordinate coord, float radius, unsigned int day)
{
		const geopop::Coordinate stride_coord = {coord.longitude(), coord.latitude()};

		const PopData popdata = m_model_ptr->GetPopulationInRadius(stride_coord, radius, day);

		m_view_ptr->DisplayPopDataInSidebar(popdata);
}

void Controller::SelectRectangular(QGeoCoordinate pointA, QGeoCoordinate pointB, unsigned int day)
{
		const geopop::Coordinate stride_pointA = {pointA.longitude(), pointA.latitude()};
		const geopop::Coordinate stride_pointB = {pointB.longitude(), pointB.latitude()};

		const PopData popdata = m_model_ptr->GetPopulationInBox(stride_pointA, stride_pointB, day);

		m_view_ptr->DisplayPopDataInSidebar(popdata);
}

} // namespace datavis
} // namespace stride
