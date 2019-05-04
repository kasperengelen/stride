import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtLocation 5.3
import QtPositioning 5.6
import QtQuick.Dialogs 1.3

import stride.datavis.controller 1.0
import stride.datavis.view 1.0

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
    
    // keeps track of the currently loaded epi data
    property var epiData : [];
    
    // C++ controller object
    Controller {
    	id: controller
    	objectName: "controller"
    }
    
    // C++ view object
    View {
    	id: view
    	objectName: "view"
    }
    
    /**
     * Save the current map contents to a file with
     * the specified filename.
     */
    function saveMap(filename)
   	{
    	map.grabToImage(function(result) {
   			result.saveToFile(filename);
   		});
   	}
   	
   	/**
   	 * Retrieve the epi-data currently stored in the model. This will
     * also adjust the slider to the model data.
   	 */
   	function loadEpiData()
   	{
   		// retrieve data from view
   		mainWindow.epiData = view.epi_data
   		
   		// set slider parameters
   		daySlider.value = 0
   		daySlider.from = 0
   		daySlider.to = mainWindow.epiData.length - 1
   		
   		// now that epi data is present, it is safe to use the slider
   		daySlider.enabled = true
    }
    
    /**
     * Display the current day on the map.
     */
    function displayCurrentDay()
    {
    	// clear previous day
    	map.clearMapItems();
    
    	// retrieve data
    	var locality_list = mainWindow.epiData[daySlider.value]
    	
		for(var key in locality_list)
		{
			var loc = locality_list[key]
			var disease_status_frac = loc.college.exposed / loc.college.total;
			// var disease_status_frac = locality_data.total.exposed / locality_data.total.pop
			
			// create tooltip to display locality name
			//var mouse_area = Qt.createQmlObject('import 
			
			// create map marker
	        var marker = Qt.createQmlObject(
	        	'import QtLocation 5.3\n'
	        	+ 'import QtQuick 2.12\n'
	        	+ 'import QtQuick.Controls 2.5\n'
	        	+ 'MapCircle {\n'
	        	+ '    id: localityMarker_' + key + ';\n'
	        	+ '    ToolTip.visible: localityMarker_' + key + '_ma.containsMouse; \n'
	        	+ '    MouseArea {\n' // add mouse interaction
	        	+ '        id: localityMarker_' + key + '_ma;\n'
	        	+ '        anchors.fill: parent;\n'
	        	+ '        hoverEnabled: true;\n'
	        	+ '        onClicked: console.log("clicked loc ' + key + '");\n'
	        	+ '    }\n'
	        	+ '}',
	        		map);
		        marker.center = QtPositioning.coordinate(loc.lat, loc.lon)
		        marker.radius = loc.totPop // TODO: change to loc.total.pop
		        marker.color  = Qt.hsva(disease_status_frac, 1.0, 1.0, 1.0)
		        marker.border.width = 0
		        marker.ToolTip.text = loc.name
		        map.addMapItem(marker)
		}
		
		map.fitViewportToMapItems()
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
            
			/**
			 * Open file button
			 */
            ToolButton {
                id: open_file
                Image {
                    source: "../img/open_simulation_file.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: {
                	controller.OpenFile()
                	
                	// TODO move this to a signal
                	
                	loadEpiData()
                	displayCurrentDay()
                }
            } // open file button

			/**
			 * Save button
			 */
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
            } // save button

            ToolSeparator {}
            
            Slider {
            	// anchor
            	// dynamic width calculation
                id: daySlider
            	from: 0
            	to: 2
            	stepSize: 1
            	implicitWidth: mainWindow.width * (3/5)
            	enabled: false
            	
            	onValueChanged: {
	            	displayCurrentDay()
            	} // value changed signal

			    background: Rectangle {
			        x: daySlider.leftPadding
			        y: daySlider.topPadding + daySlider.availableHeight / 2 - height / 2
			        implicitWidth: 200
			        implicitHeight: 4
			        width: daySlider.availableWidth
			        height: implicitHeight
			        radius: 2
			        color: "#bdbebf"
			
			        Rectangle {
			            width: daySlider.visualPosition * parent.width
			            height: parent.height
			            color: "#21be2b"
			            radius: 2
			        }
			    } // background
    
    
            	
            } // Slider
        } // RowLayout
    } // ToolBar
} // Window





