

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
	        	+ 'MapCircle {\n'
	        	+ '    id: localityMarker_' + key + ';\n'
	        	+ '    ToolTip.visible: localityMarker_' + key + '_ma.containsMouse; \n'
	        	+ '    MouseArea {\n' // add mouse interaction
	        	+ '        id: localityMarker_' + key + '_ma;\n'
	        	+ '        anchors.fill: parent;\n'
	        	+ '        hoverEnabled: true;\n'
	        	+ '        onClicked: console.log("clicked loc ' + key + '");\n'
	        	+ '    }\n'
	        	+ '}', target_map);
		        marker.center = QtPositioning.coordinate(loc.lat, loc.lon)
		        marker.radius = loc.total.pop
		        marker.color  = Qt.hsva(health_frac, 1.0, 1.0, 1.0)
		        marker.border.width = 0
		        marker.ToolTip.text = loc.name
		        target_map.addMapItem(marker)
		}
		
		// if needed, adjust map view to locality positions
		if(reset_viewport) {
			target_map.fitViewportToMapItems()
		}
    }






