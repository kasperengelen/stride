import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtLocation 5.3
import QtPositioning 5.6
import QtQuick.Dialogs 1.3

// TODO components in seperate qml files
// TODO render from file data

// main window
Window {
    width: 768
    height: 768
    visible: true
    title: "Simulation visualizer"

    FileDialog {
        id: dialog_openSimulationFile
        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: ["CSV files (*.csv)", "Sim files (*.sim)", "All files (*)"]
        selectMultiple: false

        onAccepted: {
            console.log("You chose: " + dialog_openSimulationFile.fileUrls)
            // open file, process, order by fraction of infected and call functions
            // to render localities
            
            // send file to reader
            // reader returns a EpiOutputModel, save this to somewhere
            
            // set current timestep to zero
            
            // render current timestep
        }
        onRejected: {
            console.log("Canceled")
            // TODO popup with "no file selected.
        }
        Component.onCompleted: visible = false
    }

    // toolbar at the top of the window
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
                    source: "img/open_simulation_file.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
                onClicked: dialog_openSimulationFile.open()
            }

            ToolButton {
                id: save_to_img
                Image {
                    source: "img/save_to_image.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ToolSeparator {}

            ToolButton {
                id: controls_to_begin
                Image {
                    source: "img/controls_back_full.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ToolButton {
                id: controls_previous
                Image {
                    source: "img/controls_back_one.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ToolButton {
                id: controls_play_pause
                Image {
                    source: "img/controls_auto_play.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ToolButton {
                id: controls_next
                Image {
                    source: "img/controls_forward_one.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ToolButton {
                id: controls_to_end
                Image {
                    source: "img/controls_forward_full.png"
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }

            ////////////////////////////////////////////
        }
    }

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        id: map
        plugin: mapPlugin
        // TODO center?

        // positioning: bottom of the window, in the place that is left over by the toolbar
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - toolbar.height

        // map details
        center: QtPositioning.coordinate(50.85045, 4.34878) // brussels
        zoomLevel: 8

        MapCircle {
            id: locality_brussels
            center {
                latitude: 50.85045
                longitude: 4.34878
            }
            radius: 18000.0
            color: Qt.hsva(0.2, 1.0, 1.0, 1.0)
            border.width: 0
        }

        MapCircle {
            id: locality_hasselt
            center {
                latitude: 50.93069
                longitude: 5.33248
            }
            radius: 8000.0
            color: Qt.hsva(0.2, 1.0, 1.0, 1.0)
            border.width: 0
        }

        MapCircle {
            id: locality_gent
            center {
                latitude: 51.0543422
                longitude: 3.7174243
            }
            radius: 10000.0
            color: Qt.hsva(0.2, 1.0, 1.0, 1.0)
            border.width: 0
            
            ToolTip.visible: ma.containsMouse
            ToolTip.text: qsTr("Gent: 10,000 inhabitants.\n 20% infected.")
            
            MouseArea {
            	id: ma
            	anchors.fill: parent
            	hoverEnabled: true
            }
            
        }

        MapCircle {
            id: locality_mechelen
            center {
                latitude: 51.025876
                longitude: 4.477536
            }
            radius: 10000.0
            color: Qt.hsva(0.5, 1.0, 1.0, 1.0)
            border.width: 0
        }

        MapCircle {
            id: locality_antwerp
            center {
                latitude: 51.219448
                longitude: 4.402464
            }
            radius: 14000.0
            color: Qt.hsva(1.0, 1.0, 1.0, 1.0)
            border.width: 0
        }
    } // Map


}





