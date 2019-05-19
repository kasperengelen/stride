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
        width: parent.width
        height: parent.height - toolbar.height
    } // Map

    Rectangle {    
    	id: sidebar
    	
    	width: 0
    	
        
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: parent.height - toolbar.height
        
        color: 'lightgrey'
        
        visible: false
        
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
        
        function setLocation(location, day)
        {
	        Logic.addLocationToListModel(locationModel, location)
	        locationNameDisplay.text = location.name
	        currentDayDisplay.text = "Day " + day
	        
        }
        
        /**
         * Rectangle that contains the content of the sidebar. This also
         * handles centering and margins.
         */
        Rectangle {
        	id: sidebarContent
			
			anchors.horizontalCenter: parent.horizontalCenter;
	        anchors.verticalCenter : parent.verticalCenter; 
	        width: sidebar.width - 40
	        height: sidebar.height - 40
	        
	        color: parent.color
        
				Text {
					id: locationNameDisplay; 
					
					width: sidebarContent.width
					height: 22
					
					anchors.top: parent.top

					font.pixelSize: 20;
				}
				
				Text {
					id: currentDayDisplay
					
					width: sidebarContent.width
					height: 22
					
					anchors.top: locationNameDisplay.bottom

					font.pixelSize: 20;
				}

			
			    ListView {
			    
					ScrollBar.vertical: ScrollBar {
			       		active: true
			       		policy: ScrollBar.AlwaysOn
			        }
			        
			        boundsBehavior: Flickable.StopAtBounds
		        	
					height: parent.height - locationNameDisplay.height - currentDayDisplay.height
					width: sidebarContent.width
					
					anchors.bottom: parent.bottom 
			
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
		
		} // sidebar content
        
        
        
    } // sidebar
    
    Rectangle {
    	id: sidebarToggleButton
    	
    	width: 25
    	
    	height: parent.height - toolbar.height
    	
    	color: 'grey'
    	
        anchors.right: sidebar.left
        anchors.bottom: parent.bottom
        
        Text { 
        	text : sidebar.visible ? ">" : "<";
			font.pixelSize: 30
			font.bold: true
        	anchors.horizontalCenter: parent.horizontalCenter;
        	anchors.verticalCenter : parent.verticalCenter; 
        }
        
        MouseArea {
        	anchors.fill: parent
        	onClicked: {
				sidebar.visible = !sidebar.visible
        		console.log("click")
        	}
				
        
        }
    
    }

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
	            	Logic.displayCurrentDay(false, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId, sidebar)
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
					Logic.displayCurrentDay(false, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId, sidebar)
				}        
            
            } // ComboBox
        } // RowLayout
    } // ToolBar
} // Window





