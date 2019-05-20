import QtLocation 5.3
import QtQuick 2.12
import QtQuick.Controls 2.5
import QtPositioning 5.6

import "view_code.js" as Logic

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
			var component = Qt.createComponent("stat_popup.qml")
			if(component.status != Component.Ready)
			{
				if(component.status == Component.Error)
					console.log("Error: " + component.errorString())
				else
					console.log("Error: unknown error.")
			}			
			
			var stat_popup = component.createObject(parent, {"par": parent, "location": location, "day": day})

			stat_popup.x = Logic.getAbsolutePosition(parent).x
			stat_popup.y = Logic.getAbsolutePosition(parent).y

			stat_popup.show();
			
			sidebar.setLocation(location, day)
		}
		
		onEntered: {
			sidebar.setLocation(location, day)
		}
	}
}