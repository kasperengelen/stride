

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
    function displayCurrentDay(reset_viewport, target_map, epi_data, current_day, health_status_name, sidebar)
    {
    	// clear previous day
    	target_map.clearMapItems();
    
    	// retrieve data
    	var locality_list = epi_data[current_day]
    	
    	// iterate over localities and add to map
		for(var key in locality_list)
		{
			var loc = locality_list[key]
			
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
	        var marker = marker_component.createObject(target_map, {"location": loc, "day": current_day, "sidebar": sidebar, "health_status" : health_status_name})   
		    target_map.addMapItem(marker)
		}
		
		// if needed, adjust map view to locality positions
		if(reset_viewport) {
			target_map.fitViewportToMapItems()
		}
    }
    
    /**
     * Set the specified ListModel to display the specified Location. All
     * previously present data will be cleared.
     */
    function addLocationToListModel(list_model, location)
    {
    	list_model.clear()
    
    	addPopSectionToListModel(list_model, "Total", location.total)
    	addPopSectionToListModel(list_model, "Household", location.household)
    	addPopSectionToListModel(list_model, "K12 School", location.k12school)
    	addPopSectionToListModel(list_model, "College", location.college)
    	addPopSectionToListModel(list_model, "Workplace", location.workplace)
    	addPopSectionToListModel(list_model, "Primary Community", location.primCom)
    	addPopSectionToListModel(list_model, "Secondary Community", location.secCom)
    	addPopSectionToListModel(list_model, "Daycare", location.daycare)
    	addPopSectionToListModel(list_model, "Preschool", location.preschool)
    
    }
    
    function fracToPct(frac)
    {
    	return (frac * 100).toFixed(2) + "%"
    }
    
    function addPopSectionToListModel(list_model, section_name, section_data)
    {
    	list_model.append({
    		"attrName": "Population",
    		"value": section_data.pop.toString(),
    		"popSection": section_name
    	})

    	list_model.append({
    		"attrName":   "Susceptible",
    		"value":      fracToPct(section_data.susceptible),
    		"popSection": section_name
    	})
   
    	list_model.append({
    		"attrName":   "Infected",
    		"value":      fracToPct(section_data.infected),
    		"popSection": section_name
    	})

    	list_model.append({
    		"attrName":   "Symptomatic",
    		"value":      fracToPct(section_data.symptomatic),
    		"popSection": section_name
    	})

    	list_model.append({
    		"attrName":   "Infectious",
    		"value":      fracToPct(section_data.infectious),
    		"popSection": section_name
    	})

    	list_model.append({
    		"attrName":   "Recovered",
    		"value":      fracToPct(section_data.recovered),
    		"popSection": section_name
    	})

    	list_model.append({
    		"attrName":   "Immune",
    		"value":      fracToPct(section_data.immune),
    		"popSection": section_name
    	})
    }






