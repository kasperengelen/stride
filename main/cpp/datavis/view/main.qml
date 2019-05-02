import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtLocation 5.3
import QtPositioning 5.6
import QtQuick.Dialogs 1.3

import stride.datavis.controller 1.0
import stride.datavis.model 1.0

// TODO components in seperate qml files
// TODO render from file data

// main window
Window {
	id: mainWindow
	objectName: "mainWindow"
    width: 768
    height: 768
    visible: true
    title: "Simulation visualizer"
    
    // keeps track of the currently displayed day
    property int current_day : 0;
    
    // C++ controller object
    Controller {
    	id: controller
    	objectName: "controller"
    }
    
    // C++ model object
    Model {
    	id: model
    	objectName: "model"
    }
    
    function saveMap()
   	{
    	map.grabToImage(function(result) {
   			result.saveToFile("something.png");
   		});
   	}
    
    function refreshMap()
    {
    	map.clearMapItems()
    	
		var locality_list = model.loc_list
		
		console.log("Localities")
		for(var key in locality_list)
		{
			var locality_data = locality_list[key]
			
			console.log("---")
			console.log(locality_data.name)
			console.log(locality_data.lat)
			console.log(locality_data.lon)
			console.log("total:" + locality_data.totPop)
			console.log("exposed:" + locality_data.college.exposed)
			
			var disease_status_frac = locality_data.college.exposed / locality_data.college.total;
			console.log("frac:" + frac)
			
	        var circle_1 = Qt.createQmlObject('import QtLocation 5.3; MapCircle {}', map);
		        circle_1.center = QtPositioning.coordinate(locality_data.lat, locality_data.lon)
		        circle_1.radius = locality_data.totPop
		        circle_1.color = Qt.hsva(disease_status_frac, 1.0, 1.0, 1.0)
		        circle_1.border.width = 0
		        map.addMapItem(circle_1)
		}
		
		map.fitViewportToMapItems()
	
      	
	/*
	
	
	        ToolTip.visible: ma.containsMouse
            ToolTip.text: qsTr("Gent: 10,000 inhabitants.\n 20% infected.")
            
            MouseArea {
            	id: ma
            	anchors.fill: parent
            	hoverEnabled: true
            }
            */
	 

    }
    
    /**
     * Map that displays the localities. The map will be located at the bottom of the window.
     */
	Map {
        id: map
        objectName: "map"

        plugin: Plugin {
	        id: mapPlugin
	        name: "osm"
	    }
	    
		// better when saving to file.
	    copyrightsVisible: false

        // positioning: bottom of the window, in the place that is left over by the toolbar
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - toolbar.height
    } // Map

    /**
     * Toolbar that contains the controls. The toolbar will be located at the top of the window.
     */
    ToolBar {
        // position at the top of the window, full width of the window, 50 pixels height
        anchors.top: parent.top
        width: parent.width
        height: 50
        id: toolbar
        RowLayout {
            anchors.fill: parent.top
            ToolButton {
                id: open_file
                Image {
                    source: "../img/open_simulation_file.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: {
                	controller.OpenFile()
                	// check if openfile was successful
                	
                	refreshMap()
                }
            }

            ToolButton {
                id: save_to_img
                Image {
                    source: "../img/save_to_image.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: { 
                	controller.SaveFile()
                }
            }

            ToolSeparator {}

            ToolButton {
                id: controls_to_begin
                Image {
                    source: "../img/controls_back_full.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: {
                	controller.FirstStep()
                		
                	// check if successfull
                	
                	refreshMap()
                }
            }

            ToolButton {
                id: controls_previous
                Image {
                    source: "../img/controls_back_one.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                
                onClicked: {
                	controller.PrevStep()
                	
                	// check if successfull
                	
                	refreshMap()
                }
            }

            ToolButton {
                id: controls_next
                Image {
                    source: "../img/controls_forward_one.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                
                onClicked: {
                	controller.NextStep()
                	
                	// check if successfull
                	
                	refreshMap()
                }
            }

            ToolButton {
                id: controls_to_end
                Image {
                    source: "../img/controls_forward_full.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: {
                	controller.LastStep()
                	
                	// check if successfull
                	
                	refreshMap()
                }
            }

            ////////////////////////////////////////////
        }
    }
}





