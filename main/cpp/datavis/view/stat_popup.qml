import QtQuick 2.12
import QtQuick.Window 2.2

Window {

	width: 300
	height: 200
	flags: Qt.WindowStaysOnTopHint
	
	property var currently_active : false
	
	// window title
	// day
    title: "ANTWERPEN - Day 216"
    

	
	Rectangle {
	
	    anchors.fill: parent
	
	    color: "lightGrey"
	
	    Text {
	
	        anchors.centerIn: parent
	
	        text: "test"
	    }
	}	
	
	onActiveChanged: {
		currently_active = !currently_active
		console.log("active changed.")
		console.log("cur active: " + this.currently_active)
		console.log("active: " + this.active)
		if(!this.currently_active)
		{
			this.close()
		}
	}

}