/**
 * File that contains the main window of the data visualiser application.
 */

import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtLocation 5.3
import QtPositioning 5.6
import QtQuick.Dialogs 1.3

// logic code in javascript
import "view_code.js" as Logic

// stride classes
import stride.datavis.controller 1.0
import stride.datavis.view 1.0

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
    	
		onFileReadSuccessful: {
            // if the read was completed -> refresh data, display new data
			Logic.loadEpiData(mainWindow, view, daySlider, healthTypeSelector)
			Logic.displayCurrentDay(true, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId, sidebar)
		}
		
		onSaveMapToFile: {
			Logic.saveMap(map, filename)
		}
    }
    
    // C++ view object
    View {
    	id: view
    	objectName: "view"
    }
    
    Rectangle {
    	property var 
    
    	id: sidebar
    	
    	width: 300
    	
    	height: parent.height - toolbar.height
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        
        color: 'purple'
        
        visible: true
        
        onVisibleChanged: {
        	if(!this.visible)
        	{
        		this.width = 0
        	}
        	else
        	{
        		this.width = 300
        	}
        }
        
        
        
    }
    
    Rectangle {
    	id: sidebarToggleButton
    	
    	width: 50
    	
    	height: parent.height - toolbar.height
    	
    	color: 'red'
    	
        anchors.right: sidebar.left
        anchors.bottom: parent.bottom
        
        MouseArea {
        	anchors.fill: parent
        	onClicked: {
				sidebar.visible = !sidebar.visible
        		console.log("click")
        	}
				
        
        }
    
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
        anchors.left: parent.left
        width: parent.width - sidebar.width - sidebarToggleButton.width
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
                	controller.OpenFile() // trigger the file opening dialog and subsequent reader mechanism
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
	            	Logic.displayCurrentDay(false, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId)
            	}

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
            
            /**
             * Dropdown menu to select which health status should be displayed on the map.
             */
            ComboBox {
            	id: healthTypeSelector
            	property string currentHealthId: healthTypeList.get(0).internal_name
            	
            	currentIndex: 0
            	anchors.right: parent.right
            	anchors.top: parent.top
            	model: ListModel {
            		id: healthTypeList
            		ListElement { text: "Immune";      internal_name: "immune" }
            		ListElement { text: "Susceptible"; internal_name: "susceptible" }
            		ListElement { text: "Infected";    internal_name: "infected" }
            		ListElement { text: "Symptomatic"; internal_name: "symptomatic" }
            		ListElement { text: "Infectious";  internal_name: "infectious" }
            		ListElement { text: "Recovered";   internal_name: "recovered" }
            	}
            	
            	textRole: 'text'
				
				onCurrentIndexChanged: {
					currentHealthId = healthTypeList.get(currentIndex).internal_name
					Logic.displayCurrentDay(false, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId)
				}        
            
            } // ComboBox
        } // RowLayout
    } // ToolBar
} // Window





