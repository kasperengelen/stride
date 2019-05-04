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
#include "datavis/readers/JSONReader.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>
#include <iostream>

namespace stride {
namespace datavis {

void Controller::SaveFile() {
	qDebug("save");
	QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());
	QString filename = QFileDialog::getSaveFileName(parent_ptr, tr("Save visualisation"), "", tr("Zip file (*.zip);;GIF file (*.gif)"));

	if(filename.isNull())
	{
		qDebug("no savefile selected.");
	}
	else
	{
		qDebug("filename '" + filename.toLatin1() + "'");
	}
}

void Controller::OpenFile() {
	qDebug("open");
	QWidget* parent_ptr = dynamic_cast<QWidget*>(this->parent());
	QString filename = QFileDialog::getOpenFileName(parent_ptr, tr("Open epi-data"), "", tr("JSON file (*.json);;HDF5 file (*.h5);;Protobuf file (*.proto)"));

	if(filename.isNull())
	{
		return;
	}

	qDebug("filename '" + filename.toLatin1() + "'");

	// TODO error handling: if an error occurred during reader => notify with QMessageBox
	// determine HDF5, Protobuf, JSON
	if(filename.endsWith(".json"))
	{
		// JSON
		std::ifstream infile;
		JSONReader reader(filename.toStdString());

		reader.ReadIntoModel(*m_model_ptr);

		emit this->fileReadSuccessful();

		return;
	}
	else
	{
		// Unknown format
		QMessageBox::warning(parent_ptr, tr("Warning"), tr("Specified file format is not supported."));
		return;
	}
}

}
 // namespace datavis
}// namespace stride

