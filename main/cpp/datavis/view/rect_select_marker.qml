import QtLocation 5.3; 
import QtQuick 2.12; 

/**
 * Pin marker that can be used to pick the corners
 * of a rectangular selection.
 */
MapQuickItem { 
	anchorPoint.x : image.width/4; 
	anchorPoint.y: image.height; 

	sourceItem: Image { 
		id: image
		source: "../img/map_marker.png" 
	}
}

