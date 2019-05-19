import QtLocation 5.3
import QtQuick 2.12
import QtQuick.Controls 2.5

import "view_code.js" as Logic

MapCircle {
	property var location; // the location that the marker represents
	property var day;      // the number of the day
	property var sidebar;

	ToolTip.visible:ma.containsMouse;
	ToolTip.text: location.name

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
			
			var stat_popup = component.createObject(parent)
			var parent_pos = Logic.getAbsolutePosition(parent)
			stat_popup.x = parent_pos.x
			stat_popup.y = parent_pos.y
			stat_popup.title = location.name + " - Day " + day
			stat_popup.show();
			
			sidebar.current_location = location
			sidebar.visible = true
			sidebar.width = 300
		}
	}
}