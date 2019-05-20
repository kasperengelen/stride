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

// logic code in javascript
import "logic.js" as Logic

// stride classes
import stride.datavis.controller 1.0
import stride.datavis.view 1.0

// main window
Window {
	// keeps track of the currently loaded epi data
    property var epiData : [];
	
	id: mainWindow
	objectName: "mainWindow"
    
    width: 768
    height: 768
    
    visible: true
    title: "Simulation visualizer"
    

    
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
    
    /**
     * Mouse area that handles selection of locations on the map.
     */
    MouseArea {
    	// property
    	property var radius_marker;
    	property var select_mode: "DISABLED"; // can be "DISABLED", "RADIUS", "RECTANGLE"
    	property var rad_dialog; // reference to window that helps with selection

    	id: selectionManager
    	
    	// conditional size to prevent interference with normal functionality
    	height: enabled ? map.height : 0
    	width: enabled ? (parent.width - sidebar.width - sidebarToggleButton.width) : 0
    	
    	anchors.left: parent.left
    	anchors.bottom: parent.bottom
		
		hoverEnabled: true
		enabled: false // set this to true to enter radial selection mode
		
		cursorShape: Qt.PointingHandCursor

		onEntered: {
			map.gesture.enabled = false
		}
			
		onExited: {
			map.gesture.enabled = true
		}
			
		onClicked: {
			map.removeMapItem(radius_marker)

			radius_marker = Qt.createQmlObject('import QtLocation 5.3; MapCircle {}', map)
			radius_marker.center = map.toCoordinate(Qt.point(mouse.x, mouse.y))
			radius_marker.radius = rad_dialog.getRadius()
			radius_marker.color = Qt.rgba(0.66, 0.84, 0.43, 0.6)
			radius_marker.border.width = 2
				
			map.addMapItem(radius_marker)
		}

		/**
		 * Command the mouse area to enter circular selection. This will
		 * open a selection window and enable the mouse area.
		 */
		function enterCircSelectionMode()
		{
        	var component = Qt.createComponent("radius_select_dialog.qml")
        	
			if (!Logic.checkComponent(component))
			{
				return;
			}
	
			// set mouse area as parent so that it can be accessed.
        	rad_dialog = component.createObject(selectionManager, {"selectionManager": selectionManager})
        	
        	rad_dialog.x = mainWindow.x + mainWindow.width / 2
        	rad_dialog.y = mainWindow.y + mainWindow.height / 2 - rad_dialog.height / 2
        	
        	rad_dialog.show()

        	enabled = true
            select_mode = "RADIUS"
            daySlider.enabled = false
		}

        /**
         * Command the mouse area to enter rectangular selection. This will
         * open a selection window and enable the mouse area.
         */
		function enterRectSelectionMode()
		{
            // show window

            enabled = true
            select_mode = "RECTANGLE"
            daySlider.enabled = false
		}

        /**
         * Command the mouse area to exit selection mode. This will
         * remove any map markers and disable the mouse area.
         */
		function exitSelectionMode()
		{
            if(select_mode == "RADIUS") {
    			map.removeMapItem(radius_marker)	
    			rad_dialog = null
			} else if (select_mode == "RECTANGLE") {


            }

            select_mode = "DISABLED"
            enabled = false
            daySlider.enabled = true
		}

        function updateCircRadius()
        {
            if(radius_marker != null && radius_marker != undefined) {
                radius_marker.radius = rad_dialog.getRadius()
            }
        }

        /**
         * Determine whether or not a selection is finished.
         */
        function isSelectionMade()
        {
            if(select_mode == "RADIUS") {
                return (radius_marker != undefined && radius_marker != null)
            } else if(select_mode == "RECTANGLE") {
                return false; // TODO update, make sure that two points are present
            }

            return false;
        }

        function confirmSelection()
        {
            if(select_mode == "RADIUS") {
                var coord  = radius_marker.center
                var radius = radius_marker.radius
                var day    = daySlider.value

                controller.SelectRadius(coord, radius, day)
                // TODO retrieve return value?
            } else if (select_mode == "RECTANGLE") {

            }
        }
    } // selectionManager

    /**
     * Rectangle that represents the entire sidebar.
     */
    Rectangle {    
    	id: sidebar
    	
    	width: 0
    	
    	/**
    	 * This mouse area covers the sidebar to prevent interaction with te map underneath.
    	 * The ListView is specified on top of this, so that mouse commands are not blocked.
    	 */
		MouseArea {
			anchors.fill: parent
			hoverEnabled: true
			
			onEntered: {
				map.gesture.enabled = false
			}
			
			onExited: {
				map.gesture.enabled = true
			}
		} // MouseArea
    	
        
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
        
    		/**
    		 * Displays the name of the location.
    		 */
			Text {
				id: locationNameDisplay; 
				
				width: sidebarContent.width
				height: 22
				
				anchors.top: parent.top

				font.pixelSize: 20;
			}

    		/**
    		 * Displays the current day.
    		 */
			Text {
				id: currentDayDisplay
				
				width: sidebarContent.width
				height: 22
				
				anchors.top: locationNameDisplay.bottom

				font.pixelSize: 20;
			}

			/**
			 * Displays the entire list.
			 */
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
		    
		    /**
		     * Displays population section attributes.
		     */
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

		    /**
			 * Displays population section names.
			 */
		    Component {
		        id: popSectionDelegate
		
		        Rectangle {
		            width: ListView.view.width
		            height: 22
		            Text{ text: section; font.pixelSize: 20; }
		        }
		    }
		
			// contains the epi data of the location
		    ListModel {
		        id: locationModel
		    }
		} // sidebar content 
    } // sidebar
    
    /**
     * Toggle button to show/hide the sidebar
     */
    Rectangle {
    	id: sidebarToggleButton
    	
    	width: 25
    	
    	height: parent.height - toolbar.height
    	
    	color: 'grey'
    	
        anchors.right: sidebar.left
        anchors.bottom: parent.bottom
        
        /**
         * Arrow symbol.
         */
        Text { 
        	text : sidebar.visible ? ">" : "<";
			font.pixelSize: 30
			font.bold: true
        	anchors.horizontalCenter: parent.horizontalCenter;
        	anchors.verticalCenter : parent.verticalCenter; 
        }
        
    	/**
    	 * This mouse area covers the sidebar toggle button to 
    	 * prevent interaction with te map underneath.
    	 */
        MouseArea {
        	anchors.fill: parent
			hoverEnabled: true

        	onClicked: {
				sidebar.visible = !sidebar.visible
        	}

			// disable map interaction when over this mousearea
			onEntered: {
				map.gesture.enabled = false
			}
			
			onExited: {
				map.gesture.enabled = true
			}
        } // MouseArea
    } // sidebarToggleButton

    /**
     * Toolbar that contains the controls. The toolbar will be located at the top of the window.
     */
    ToolBar {
        id: toolbar

        anchors.top: parent.top
        
        width: parent.width
        height: 50
        
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

            /**
             * Enter circular selection mode
             */
            ToolButton {
                id: select_circ
                
                Image {
                    source: "../img/select_circle.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }

                onClicked: selectionManager.enterCircSelectionMode()
            } // circular selection button
            
            /**
             * Enter rectangular selection mode
             */
            ToolButton {
                id: select_rect
                Image {
                    source: "../img/select_rect.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: selectionManager.enterRectSelectionMode()
            } // rectangular selection button
            
            ToolSeparator {}
            
            /**
             * Slide to select a day from the simulation.
             */
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





