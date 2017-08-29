module.exports = [
  {
    "type": "heading",
    "defaultValue": "Pip Boy 300 Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Configure your Pip Boy"
  },
  {
    "type": "section",
    "items": [
			{
  			"type": "toggle",
  			"messageKey": "CRIPPLED_STATUS",
  			"label": "Show crippled status?",
  			"defaultValue": 0
			}
		]
	},
	
	{
    "type": "section",
    "items": [
			{
  			"type": "radiogroup",
  			"messageKey": "STEPS_TYPE",
				"label": "Choose a step goal:",
  			"options": [
    			{ 
      			"label": "Better than yesterday",
      			"defaultValue": "0"
    			},
    			{ 
      			"label": "Better than your daily average",
      			"value": "1" 
    			},
					{ 
      			"label": "Better than your weekly average",
      			"value": "2" 
    			},
    			{ 
      			"label": "Better than your monthly average",
      			"value": "3" 
    			},
    			{ 
						"label": "Set a manual goal",
      			"value": "4" 
    			}
  			]
			}
		]
	},
	
	{
    "type": "section",
    "items": [
			{
  			"type": "input",
  			"messageKey": "MANUAL_STEPS",
  			"defaultValue": "10000",
  			"label": "Manual Step Goal:",
  			"attributes": {
    			"placeholder": "10000",
    			"limit": 5,
    			"type": "steps"
  			}
			}
		]
	},
	
  {
    "type": "section",
    "items": [
			{
  			"type": "radiogroup",
  			"messageKey": "SLEEP_TYPE",
				"label": "Choose a sleep goal:",
  			"options": [
    			{ 
      			"label": "Better than yesterday", 
      			"defaultValue": "0"
    			},
    			{ 
      			"label": "Better than your daily average", 
      			"value": "1" 
    			},
    			{ 
      			"label": "Better than your weekly average", 
      			"value": "2" 
    			},
    			{ 
      			"label": "Better than your monthly average", 
      			"value": "3" 
    			},
    			{ 
						"label": "Set a manual goal", 
      			"value": "4" 
    			}
  			]
			}
		]
	},	
	
	{
    "type": "section",
    "items": [
			{
			  "type": "slider",
 			  "messageKey": "MANUAL_SLEEP",
				"label": "Manual Sleep Goal:",
  			"defaultValue": 8,
  			"min": 1,
  			"max": 24,
  			"step": 1
			}
    ]
  },
	
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];