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
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include "../readers/JSONEpiReader.h"
#include "datavis/readers/EpiReaderException.h"

namespace stride {
namespace datavis {

void Controller::SaveFile() {
	QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());

	QMessageBox::critical(parent_ptr, tr("Warning"), tr("Saving to image files is not yet supported."));
	return;

	QString filename = QFileDialog::getSaveFileName(parent_ptr, tr("Save visualisation"), "", tr("Zip file (*.zip);;GIF file (*.gif)"));



	if(filename.isNull())
	{
		return;
	}

	// TODO save to file
	// -> gif
	// -> zip

	return;
}

void Controller::OpenFile() {
	QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());
	QString filename = QFileDialog::getOpenFileName(parent_ptr, tr("Open epi-data"), "", tr("JSON file (*.json);;HDF5 file (*.h5);;Protobuf file (*.proto)"));

	if(filename.isNull())
	{
		return;
	}

	// determine HDF5, Protobuf, JSON
	if(filename.endsWith(".json"))
	{
		try {
			// JSON
			JSONEpiReader reader(filename.toStdString());

			reader.ReadIntoModel(*m_model_ptr);

			emit this->fileReadSuccessful();

			return;
		} catch (const EpiReaderException& e) {
			const QString err_msg = QString{"An error occurred while processing the specified file.\n"} + QString{e.what()};
			QMessageBox::critical(parent_ptr, QString{"Error"}, err_msg);
			return;
		}
	}
	else
	{
		// Unknown format
		QMessageBox::critical(parent_ptr, tr("Warning"), tr("Specified file format is not supported."));
		return;
	}

	// TODO HDF5
}

}
 // namespace datavis
}// namespace stride

