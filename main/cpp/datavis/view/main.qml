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

    minimumWidth : 768
    minimumHeight : 768
    
    visible: true
    title: "Simulation visualizer"
    
    /**
     * QML representation of the C++ Controller. This
     * object relays all the UI commands to the C++ backend.
     */
    Controller {
        id: controller
        objectName: "controller"
        
        // slot to load epi data
        onFileReadSuccessful: {
            Logic.loadEpiData(mainWindow, view, daySlider, healthTypeSelector)
            Logic.displayCurrentDay(true, map, mainWindow.epiData, daySlider.value, healthTypeSelector.currentHealthId, sidebar)
            sidebar.clearData()
        }
        
        // slot to save to file.
        onSaveMapToFile: {
            Logic.saveMap(map, filename)
        }
    }
    
    /**
     * QML representation of the C++ View. This
     * object presents information to the QML frontend.
     */
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

        /**
         * Retrieve a geographic coordinate that corresponds 
         * with the position of the mouse on the map.
         */
        function mousePosToCoord(mouse)
        {
            return map.toCoordinate(Qt.point(mouse.x, mouse.y))
        }
    }
    
    /**
     * MouseArea that handles the radius selection.
     */
    MouseArea {
        property var coord1
        property var coord2
        property var marker

        property var dialog

        id: radSelectionManager
        
        // conditional size to prevent interference with normal functionality
        height: enabled ? map.height : 0
        width: enabled ? (parent.width - sidebar.width - sidebarToggleButton.width) : 0
        
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        
        hoverEnabled: false

        enabled: false
        
        cursorShape: Qt.PointingHandCursor

        acceptedButtons: Qt.RightButton
        
        ///// DRAG & SELECT MECHANISMS /////

        onPressed: {
            map.removeMapItem(marker)

            coord1 = map.mousePosToCoord(mouse)
            coord2 = map.mousePosToCoord(mouse)

            var component = Qt.createComponent("radius_select_marker.qml")

            if (!Logic.checkComponent(component))
            {
                return;
            }

            marker = component.createObject(map)

            marker.center = coord1
            marker.radius = Logic.distanceBetween(coord1, coord2)

            map.addMapItem(marker)
        }

        onPositionChanged: {
            // only do selection if the first point has been selected
            if(coord1 != undefined && coord1 != null) {
                coord2 = map.mousePosToCoord(mouse)
                marker.radius = Logic.distanceBetween(coord1, coord2)
            }
        }

        onReleased: {
            coord2 = map.mousePosToCoord(mouse)
            marker.radius = Logic.distanceBetween(coord1, coord2)
        }

        ///// DRAG & SELECT MECHANISMS /////

        /**
         * 
         */
        function enable()
        {
            enabled = true
            daySlider.disable()

            // create dialog
            var component = Qt.createComponent("selection_dialog.qml")

            if (!Logic.checkComponent(component))
            {
                return;
            }

            dialog = component.createObject(radSelectionManager, {"selectionManager": radSelectionManager})
            
            dialog.x = mainWindow.x + mainWindow.width / 2
            dialog.y = mainWindow.y + mainWindow.height / 2 - dialog.height / 2
            
            dialog.show()
        }

        /**
         * Disable the selection manager. This will make it
         * invisible and it will stop receiving mouse events.
         */
        function disable()
        {
            enabled = false
            daySlider.enable()
        }

        /**
         * Close this dialog if the dialog exists and is opened.
         */
        function closeDialog()
        {
            if(dialog != null && dialog != undefined)
                dialog.close()
        }

        /**
         * Clear the stored selection and remove it
         * from the map.
         */
        function clear()
        {
            // remove selection
            coord1 = null
            coord2 = null

            // clear map
            map.removeMapItem(marker)
            marker = null
        }

        /**
         * Send the selection parameters to the C++
         * controller, clear the selection, and optionally
         * close the window.
         */
        function confirm(do_close_dialog)
        {
            var day = daySlider.value

            var coord  = marker.center
            var radius = marker.radius

            // close the dialog, this will also call
            // terminate()
            if(do_close_dialog)
                closeDialog()

            controller.SelectRadius(coord, radius, day)
        }

        /**
         * Ends the selection mode. This
         * will also clear the selection from the map
         * and remove any stored selection parameters.
         */
        function terminate(do_close_dialog)
        {
            if(do_close_dialog)
                closeDialog()

            // clear and disable the selection manager
            clear()
            disable()
        }

        /**
         * Determine whether the selection is valid and can be confirmed.
         */
        function validateSelection()
        {
            return coord1 != undefined && coord1 != null 
                        && coord2 != undefined && coord2 != null
                        && marker != null
        }
    } // selectionTestRadius

    /**
     * MouseArea that handles the rectangular selection.
     */
    MouseArea {
        property var coord1
        property var coord2
        property var marker

        property var dialog

        id: rectSelectionManager
        
        // conditional size to prevent interference with normal functionality
        height: enabled ? map.height : 0
        width: enabled ? (parent.width - sidebar.width - sidebarToggleButton.width) : 0
        
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        
        hoverEnabled: false

        enabled: false
        
        cursorShape: Qt.PointingHandCursor

        acceptedButtons: Qt.RightButton
        
        ///// DRAG & SELECT MECHANISMS /////

        onPressed: {
            map.removeMapItem(marker)

            coord1 = map.mousePosToCoord(mouse)
            coord2 = map.mousePosToCoord(mouse)

            var component = Qt.createComponent("rect_select_marker.qml")

            if (!Logic.checkComponent(component))
            {
                return;
            }

            marker = component.createObject(map)
            marker.setCorners(coord1, coord2)

            map.addMapItem(marker)
        }

        onPositionChanged: {
            // only do selection if the first point has been selected
            if(coord1 != undefined && coord1 != null) {
                coord2 = map.mousePosToCoord(mouse)
                marker.setCorners(coord1, coord2)
            }
        }

        onReleased: {
            coord2 = map.mousePosToCoord(mouse)
            marker.setCorners(coord1, coord2)
        }

        ///// DRAG & SELECT MECHANISMS /////

        function enable()
        {
            enabled = true
            daySlider.disable()

            // create dialog
            var component = Qt.createComponent("selection_dialog.qml")

            if (!Logic.checkComponent(component))
            {
                return;
            }

            dialog = component.createObject(rectSelectionManager, {"selectionManager": rectSelectionManager})
            
            dialog.x = mainWindow.x + mainWindow.width / 2
            dialog.y = mainWindow.y + mainWindow.height / 2 - dialog.height / 2
            
            dialog.show()
        }

        /**
         * Disable the selection manager. This will make it
         * invisible and it will stop receiving mouse events.
         */
        function disable()
        {
            enabled = false
            daySlider.enable()
        }

        /**
         * Close this dialog if the dialog exists and is opened.
         */
        function closeDialog()
        {
            if(dialog != null && dialog != undefined)
                dialog.close()
        }

        /**
         * Clear the stored selection and remove it
         * from the map.
         */
        function clear()
        {
            // remove selection
            coord1 = null
            coord2 = null

            // clear map
            map.removeMapItem(marker)
            marker = null
        }

        /**
         * Send the selection parameters to the C++
         * controller, clear the selection, and optionally
         * close the window.
         */
        function confirm(do_close_dialog)
        {
            var day = daySlider.value

            var pointA = coord1
            var pointB = coord2

            // close the dialog, this will also call
            // terminate()
            if(do_close_dialog)
                closeDialog()

            controller.SelectRectangular(pointA, pointB, day)
        }

        /**
         * Ends the selection mode. This
         * will also clear the selection from the map
         * and remove any stored selection parameters.
         */
        function terminate(do_close_dialog)
        {
            if(do_close_dialog)
                closeDialog()

            // clear and disable the selection manager
            clear()
            disable()
        }

        /**
         * Determine whether the selection is valid and can be confirmed.
         */
        function validateSelection()
        {
            return coord1 != undefined && coord1 != null 
                        && coord2 != undefined && coord2 != null
        }
    } // selectionTestRect

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

        function clearData()
        {
            locationModel.clear()
            locationNameDisplay.text = ""
            currentDayDisplay.text   = ""
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
            anchors.fill: parent
            
            /**
             * Open file button
             */
            ToolButton {
                ToolTip.visible: hovered
                ToolTip.text: "Open file"

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
                ToolTip.visible: hovered
                ToolTip.text: "Save timestep to image"

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
                ToolTip.visible: hovered
                ToolTip.text: "Select radius"

                id: select_circ
                
                Image {
                    // SOURCE: https://www.iconfinder.com/iconsets/common-toolbar
                    source: "../img/select_circle.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }

                onClicked: {
                    rectSelectionManager.disable()
                    rectSelectionManager.closeDialog()
                    radSelectionManager.disable()
                    radSelectionManager.closeDialog()

                    radSelectionManager.enable()
                }
            } // circular selection button
            
            /**
             * Enter rectangular selection mode
             */
            ToolButton {
                ToolTip.visible: hovered
                ToolTip.text: "Select box"

                id: select_rect
                Image {
                    // SOURCE: https://www.iconfinder.com/iconsets/common-toolbar
                    source: "../img/select_rect.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                
                onClicked: {
                    radSelectionManager.disable()
                    radSelectionManager.closeDialog()
                    rectSelectionManager.disable()
                    rectSelectionManager.closeDialog()

                    rectSelectionManager.enable()
                }
            } // rectangular selection button
            
            ToolSeparator {}

            /**
             * Button to toggle the timestep timer.
             */
            ToolButton {
                property var play_status: false

                id: play_pause

                enabled: false

                ToolTip.visible: hovered
                ToolTip.text: "Toggle autoplay"

                Image {
                    id: button_icon

                    // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
                    source: "../img/play.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }

                onClicked: {
                    play_status = !play_status
                }

                onPlay_statusChanged: {
                    if(play_status)
                    {
                        // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
                        button_icon.source = "../img/pause.png"
                    }
                    else
                    {
                        // SOURCE: https://www.iconfinder.com/iconsets/buttons-9
                        button_icon.source = "../img/play.png"
                    }
                }
            }
            
            /**
             * Slide to select a day from the simulation.
             */
            Slider {
                // anchor
                // dynamic width calculation
                id: daySlider
                from: 0
                stepSize: 1
                //implicitWidth: mainWindow.width * (3/5)
                enabled: false

                function enable()
                {
                    enabled = true
                    play_pause.enabled = true
                }

                function disable()
                {
                    enabled = false

                    play_pause.enabled = false
                    play_pause.play_status = false

                    // stop timer
                }

                onValueChanged: {
                    Logic.displayCurrentDay(false, map, mainWindow.epiData, value, healthTypeSelector.currentHealthId, sidebar)
                    if(value == to) {
                    	play_pause.enabled = false
                    	play_pause.play_status = false
                    } else {
                    	play_pause.enabled = true
                    }
                }

                /**
                 * MouseArea that handles the scroll wheel event so that the scroll
                 * wheel can be used to scroll through timesteps.
                 */
                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        // forward mouse event
                        mouse.accepted = false
                    }
                    
                    onReleased: {
                        // forward mouse event
                        mouse.accepted = false
                    }

                    onWheel: {
                        console.log(wheel.pixelDelta.x)
                        console.log(wheel.pixelDelta.y)

                        console.log(wheel.angleDelta.x)
                        console.log(wheel.angleDelta.y)

                        if(wheel.angleDelta.y > 0)
                        {
                            daySlider.increase()
                        }
                        else if(wheel.angleDelta.y < 0)
                        {
                            daySlider.decrease()
                        }
                    }
                }

                /**
                 * Time that automatically increments the slider
                 * if enabled.
                 */
                Timer {
                    interval: 500
                    running : play_pause.play_status
                    repeat: true

                    onTriggered: {
                        daySlider.increase()
                    }
                }

                Layout.fillWidth: true

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
                ToolTip.visible: hovered
                ToolTip.text: "Select health status"

                id: healthTypeSelector
                
                property string currentHealthId: healthTypeList.get(0).internal_name
                
                currentIndex: 0
                
                Layout.alignment: Qt.AlignRight | Qt.AlignTop

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





