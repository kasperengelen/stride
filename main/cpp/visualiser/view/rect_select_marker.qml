import QtLocation 5.3; 
import QtQuick 2.12; 

/**
 * Rectangle that indicates what portion of
 * the map has been selected.
 */
MapRectangle {
	color: Qt.rgba(0.66, 0.84, 0.43, 0.6)
    border.width: 2

    /**
     * Set the specified two points as markers between
     * which the rectangle should be drawn.
     */
    function setCorners(pointA, pointB)
    {
    	topLeft.latitude  = Math.max(pointA.latitude, pointB.latitude)
    	topLeft.longitude = Math.min(pointA.longitude, pointB.longitude)

    	bottomRight.latitude  = Math.min(pointA.latitude, pointB.latitude)
    	bottomRight.longitude = Math.max(pointA.longitude, pointB.longitude)
    }
}