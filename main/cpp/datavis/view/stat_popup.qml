import QtQuick 2.12
import QtQuick.Window 2.2
import QtQml.Models 2.12

Window {

	width: 300
	height: 200
	flags: Qt.WindowStaysOnTopHint
	
	Rectangle {
	
	    anchors.fill: parent
	
	    color: "lightGrey"
	    
	    ListView {
	        anchors.fill: parent
	        anchors.margins: 20
	
	        clip: true
	
	        model: spaceMen
	
	        delegate: spaceManDelegate
	
	        section.property: "nation"
	        section.delegate: sectionDelegate
	    }
	    
    Component {
        id: spaceManDelegate

        Item {
            width: ListView.view.width
            height: 20
            Text {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 8
                font.pixelSize: 12
                text: name
                color: '#1f1f1f'
            }
        }
    }

    Component {
        id: sectionDelegate

        Rectangle {
            width: ListView.view.width
            height: 20
            Text{ text: section }
        }
    }


    ListModel {
        id: spaceMen

        ListElement { name: "Population";  nation: "Total"; }
        ListElement { name: "Susceptible"; nation: "Total"; }
        ListElement { name: "Infected";    nation: "Total"; }
        ListElement { name: "Symptomatic"; nation: "Total"; }
        ListElement { name: "Infectious";  nation: "Total"; }
        ListElement { name: "Recovered";   nation: "Total"; }
        ListElement { name: "Immune";      nation: "Total"; }
        
        ListElement { name: "Population";  nation: "College"; }
        ListElement { name: "Susceptible"; nation: "College"; }
        ListElement { name: "Infected";    nation: "College"; }
        ListElement { name: "Symptomatic"; nation: "College"; }
        ListElement { name: "Infectious";  nation: "College"; }
        ListElement { name: "Recovered";   nation: "College"; }
        ListElement { name: "Immune";      nation: "College"; }
        
        ListElement { name: "Population";  nation: "Daycare"; }
        ListElement { name: "Susceptible"; nation: "Daycare"; }
        ListElement { name: "Infected";    nation: "Daycare"; }
        ListElement { name: "Symptomatic"; nation: "Daycare"; }
        ListElement { name: "Infectious";  nation: "Daycare"; }
        ListElement { name: "Recovered";   nation: "Daycare"; }
        ListElement { name: "Immune";      nation: "Daycare"; }
    }
	
	}	
	
	/*
	 * Mechanism to close window when it goes out of focus.
	 */
	
	// used to keep track of the currently focussed window.
	property var currently_active : false
	
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