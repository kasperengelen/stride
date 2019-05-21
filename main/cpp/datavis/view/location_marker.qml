import QtLocation 5.3
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtPositioning 5.6

import "logic.js" as Logic

MapCircle {
	property var location; // the location that the marker represents
	property var day;      // the number of the day
	property var sidebar;  // reference to the sidebar so that the epi-data in the sidebar can be updated
	property var health_status; // the health status that is being displayed by this marker

	ToolTip.visible:ma.containsMouse;
	ToolTip.text: location.name
	
	center: QtPositioning.coordinate(location.lat, location.lon)
	radius: Logic.circleSize(location.total.pop)
	color:  Qt.hsva(location.total[health_status], 1.0, 1.0, 0.5)
	border.width: 4

	MouseArea{
		id:ma

		anchors.fill:parent

		cursorShape:Qt.PointingHandCursor

		hoverEnabled:true

		onClicked: {			
			sidebar.setLocation(location, day)
		}
	}
}