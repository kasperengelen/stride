

    /**
     * Save the current map contents to a file with
     * the specified filename.
     */
    function saveMap(map_object, filename)
   	{
    	map_object.grabToImage(function(result) {
   			result.saveToFile(filename);
   		});
   	}
   	
   	/**
   	 * Retrieve the (somewhat) global position of the specified object.
   	 *
   	 * SOURCE: https://stackoverflow.com/questions/19392163/qml-mouse-absolute-position-in-mousearea?rq=1
   	 */
	function getAbsolutePosition(node) {
		var returnPos = {};
		returnPos.x = 0;
		returnPos.y = 0;
		if(node !== undefined && node !== null) {
			var parentValue = getAbsolutePosition(node.parent);
			returnPos.x = parentValue.x + node.x;
			returnPos.y = parentValue.y + node.y;
		}
		return returnPos;
	}
   	
   	/**
   	 * Retrieve the epi-data currently stored in the model. This will
     * also adjust the slider to the model data.
   	 */
   	function loadEpiData(window, view, slider, health_status_selector)
   	{
   		// retrieve data from view
   		window.epiData = view.epi_data
   		
   		// set slider parameters
   		slider.value = 0
   		slider.from = 0
   		slider.to = mainWindow.epiData.length - 1
   		
   		health_status_selector.currentIndex = 0
   		
   		// now that epi data is present, it is safe to use the slider
   		slider.enabled = true
    }
    
    function circleSize(pop_size)
    {
    	return (Math.sqrt(pop_size * 20) * 7.9) + 400
    }
    
    /**
     * Display the current day on the map.
     */
    function displayCurrentDay(reset_viewport, target_map, epi_data, current_day, health_status_name)
    {
    	// clear previous day
    	target_map.clearMapItems();
    
    	// retrieve data
    	var locality_list = epi_data[current_day]
    	
    	// iterate over localities and add to map
		for(var key in locality_list)
		{
			var loc = locality_list[key]
			var health_frac = loc.total[health_status_name]
			
			// create map marker
	        var marker = Qt.createQmlObject(
	        	  'import QtLocation 5.3\n'
	        	+ 'import QtQuick 2.12\n'
	        	+ 'import QtQuick.Controls 2.5\n'
	        	+ 'import "view_code.js" as Logic\n'
	        	+ 'MapCircle {\n'
	        	+ '    id: localityMarker_' + key + ';\n'
	        	+ '    ToolTip.visible: localityMarker_' + key + '_ma.containsMouse; \n'
	        	+ '    MouseArea {\n' // add mouse interaction
	        	+ '        id: localityMarker_' + key + '_ma;\n'
	        	+ '        anchors.fill: parent;\n'
	        	+ '        cursorShape: Qt.PointingHandCursor;\n'
	        	+ '        hoverEnabled: true;\n'
	        	+ '        onClicked: {\n'
	        	+ '            console.log("clicked loc ' + key + '");\n'
	        	+ '            var component = Qt.createComponent("stat_popup.qml");\n'
	        	+ '            var stat_popup = component.createObject(localityMarker_' + key + ');\n'
	        	+ '            var parent_pos = Logic.getAbsolutePosition(parent);\n'
	        	+ '            stat_popup.x = parent_pos.x;\n'
	        	+ '            stat_popup.y = parent_pos.y;\n'
	        	+ '            stat_popup.title = "' + loc.name + ' - Day ' + current_day + '";\n'
	        	+ '            stat_popup.show();\n'
	        	+ '        }\n'
	        	+ '    }\n'
	        	+ '}', target_map);
		        marker.center = QtPositioning.coordinate(loc.lat, loc.lon)
		        marker.radius = circleSize(loc.total.pop)
		        console.log(loc.name + " -- " + marker.radius)
		        marker.color  = Qt.hsva(health_frac, 1.0, 1.0, 0.5)
		        marker.border.width = 4
		        marker.ToolTip.text = loc.name
		        target_map.addMapItem(marker)
		}
		
		// if needed, adjust map view to locality positions
		if(reset_viewport) {
			target_map.fitViewportToMapItems()
		}
    }






