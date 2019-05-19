

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
	        var marker_component = Qt.createComponent("location_marker.qml")
	        
			if(marker_component.status != Component.Ready)
			{
				if(marker_component.status == Component.Error)
					console.log("Error: " + marker_component.errorString())
				else
					console.log("Error: unknown error.")
			}	
	        
	        // instantiate marker and pass parameters
	        var marker = marker_component.createObject(target_map, {"location": loc, "day": current_day})
	        
	        	// set visual effects
		        marker.center = QtPositioning.coordinate(loc.lat, loc.lon)
		        marker.radius = circleSize(loc.total.pop)
		        marker.color  = Qt.hsva(health_frac, 1.0, 1.0, 0.5)
		        marker.border.width = 4
		        
		        target_map.addMapItem(marker)
		}
		
		// if needed, adjust map view to locality positions
		if(reset_viewport) {
			target_map.fitViewportToMapItems()
		}
    }






