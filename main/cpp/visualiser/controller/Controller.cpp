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

#include "visualiser/controller/Controller.h"

#include "visualiser/readers/JSONEpiReader.h"
#include "visualiser/readers/HDF5EpiReader.h"
#include "visualiser/readers/ProtobufEpiReader.h"
#include "visualiser/readers/EpiReaderException.h"
#include "visualiser/readers/JSONEpiReader.h"

#include <iostream>
#include <sstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

namespace stride {
namespace visualiser {

void Controller::OpenFile()
{
        QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());
        QString  filename   = QFileDialog::getOpenFileName(
            parent_ptr, tr("Open epi-data"), "", tr("All (*.*);;JSON file (*.json);;HDF5 file (*.h5);;Protobuf file (*.proto)"));

        if (filename.isNull()) {
                return;
        }

        std::unique_ptr<EpiReader> reader_ptr;

        if (filename.endsWith(".json")) {
            reader_ptr = std::make_unique<JSONEpiReader>(filename.toStdString());
        } else if (filename.endsWith(".h5") or filename.endsWith(".hdf5")) {
            reader_ptr = std::make_unique<HDF5EpiReader>(filename.toStdString());
        } else if (filename.endsWith(".proto")) {
            reader_ptr = std::make_unique<ProtobufEpiReader>(filename.toStdString());
        } else {
            // Unknown format
            QMessageBox::critical(parent_ptr, tr("Warning"), tr("Specified file format is not supported."));
            return;
        }

        try {
            reader_ptr->ReadIntoModel(*m_model_ptr);

            emit this->fileReadSuccessful();
        } catch(const EpiReaderException& e) {
            const QString err_msg =
                    QString{"An error occurred while processing the specified file.\n"} + QString{e.what()};
            QMessageBox::critical(parent_ptr, QString{"Error"}, err_msg);
        }
}

void Controller::SaveFile()
{
        QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());

        QString filename =
            QFileDialog::getSaveFileName(parent_ptr, tr("Save visualisation"), "", tr("PNG file (*.png)"));

        if (filename.isNull()) {
                return;
        }

        if (!filename.endsWith(".png")) {
                filename += ".png";
        }

        emit this->saveMapToFile(filename);

        return;
}

void Controller::SelectRadius(QGeoCoordinate coord, float radius, unsigned int day)
{
        const geopop::Coordinate stride_coord = {coord.longitude(), coord.latitude()};

        const geopop::PopStats popstats = m_model_ptr->GetPopulationInRadius(stride_coord, radius, day);

        m_view_ptr->DisplayPopStatsInSidebar(popstats);
}

void Controller::SelectRectangular(QGeoCoordinate pointA, QGeoCoordinate pointB, unsigned int day)
{
        const geopop::Coordinate stride_pointA = {pointA.longitude(), pointA.latitude()};
        const geopop::Coordinate stride_pointB = {pointB.longitude(), pointB.latitude()};

        const geopop::PopStats popstats = m_model_ptr->GetPopulationInBox(stride_pointA, stride_pointB, day);

        m_view_ptr->DisplayPopStatsInSidebar(popstats);
}

} // namespace visualiser
} // namespace stride
