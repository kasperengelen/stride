import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5

import "view_code.js" as Logic

Window {
	property var par;

	width:  300
	height: 400
	
	x:     Logic.getAbsolutePosition(par).x
	y:     Logic.getAbsolutePosition(par).y
	
	title: location.name + " - Day " + day
	
	// load the location data
	Component.onCompleted: Logic.addLocationToListModel(locationModel, location)
	
	flags: Qt.WindowStaysOnTopHint
	    
	    ListView {
	    
			ScrollBar.vertical: ScrollBar {
	       		active: true
	       		policy: ScrollBar.AlwaysOn
	        }
	        
	        boundsBehavior: Flickable.StopAtBounds
	        
	        anchors.fill: parent
	        anchors.margins: 20
	
	        clip: true
	
	        model: locationModel
	
	        delegate: popSectionAttrDelegate
	
	        section.property: "popSection"
	        section.delegate: popSectionDelegate
	    }
	    
	    Component {
	        id: popSectionAttrDelegate
	
	        Item {
	            width: ListView.view.width
	            height: 20
	
	            Text {
	                anchors.left: parent.left
	                anchors.verticalCenter: parent.verticalCenter
	                anchors.leftMargin: 8
	                font.pixelSize: 16
	                text: attrName
	                color: '#1f1f1f'
	            }
	            
	            Text {
	                anchors.right: parent.right
	                anchors.verticalCenter: parent.verticalCenter
	                anchors.rightMargin: 8
	                font.pixelSize: 16
	                text: value
	                color: '#1f1f1f'
	            }
	        }
	    }
	
	    Component {
	        id: popSectionDelegate
	
	        Rectangle {
	            width: ListView.view.width
	            height: 22
	            Text{ text: section; font.pixelSize: 20; }
	        }
	    }
	
		
	    ListModel {
	        id: locationModel
	    }

	
	/*
	 * Mechanism to close window when it goes out of focus.
	 */
	
	// used to keep track of the currently focussed window.
	property var currently_active : false
	
	onActiveChanged: {
		currently_active = !currently_active
		var do_single_window = false;
		if(!this.currently_active && do_single_window)
		{
			this.close()
		}
	}

}