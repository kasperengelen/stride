import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12
import QtQuick.Controls 2.5


Window {

	width: 300
	height: 400
	flags: Qt.WindowStaysOnTopHint
	
	Rectangle {
	
	    anchors.fill: parent
	
	    color: "lightGrey"
	    
	    ListView {
	    
			ScrollBar.vertical: ScrollBar {
	       		active: true
	       		policy: ScrollBar.AlwaysOn
	        }
	        
	        boundsBehavior: Flickable.StopAtBounds
	        
	        anchors.fill: parent
	        anchors.margins: 20
	
	        clip: true
	
	        model: localityModel
	
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
        id: localityModel

        ListElement { attrName: "Population";  popSection: "Total"; value: 10000; }
        ListElement { attrName: "Susceptible"; popSection: "Total"; value: 0.2;   }
        ListElement { attrName: "Infected";    popSection: "Total"; value: 0.9;   }
        ListElement { attrName: "Symptomatic"; popSection: "Total"; value: 0.5;   }
        ListElement { attrName: "Infectious";  popSection: "Total"; value: 0.7;   }
        ListElement { attrName: "Recovered";   popSection: "Total"; value: 0.0;   }
        ListElement { attrName: "Immune";      popSection: "Total"; value: 0.6;   }
        
        ListElement { attrName: "Population";  popSection: "College"; value: 10000; }
        ListElement { attrName: "Susceptible"; popSection: "College"; value: 0.2;   }
        ListElement { attrName: "Infected";    popSection: "College"; value: 0.9;   }
        ListElement { attrName: "Symptomatic"; popSection: "College"; value: 0.5;   }
        ListElement { attrName: "Infectious";  popSection: "College"; value: 0.7;   }
        ListElement { attrName: "Recovered";   popSection: "College"; value: 0.0;   }
        ListElement { attrName: "Immune";      popSection: "College"; value: 0.6;   }
        
        ListElement { attrName: "Population";  popSection: "Daycare"; value: 10000; }
        ListElement { attrName: "Susceptible"; popSection: "Daycare"; value: 0.2;   }
        ListElement { attrName: "Infected";    popSection: "Daycare"; value: 0.9;   }
        ListElement { attrName: "Symptomatic"; popSection: "Daycare"; value: 0.5;   }
        ListElement { attrName: "Infectious";  popSection: "Daycare"; value: 0.7;   }
        ListElement { attrName: "Recovered";   popSection: "Daycare"; value: 0.0;   }
        ListElement { attrName: "Immune";      popSection: "Daycare"; value: 0.6;   }
    }
	
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