var temperature;
var conditions; 

var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
	var myAPIKey = "db0f8d947d39c6e0aacbd78159e1830d";
	var units = "imperial";
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude + '&units=' + units + '&appid=' + myAPIKey;

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      var json = JSON.parse(responseText);
      temperature = Math.round(json.main.temp);
      conditions = json.weather[0].main;      
      //console.log('Temperature is ' + temperature);
      //console.log('Conditions are ' + conditions);
			
			var dictionary_weather = {
  			'TEMPERATURE': temperature,
  			'CONDITIONS': conditions
			};

			Pebble.sendAppMessage(dictionary_weather,
  			function(e) {
    			console.log('Weather info sent to Pebble successfully!');
  			},
  			function(e) {
    			console.log('Error sending weather info to Pebble!');
  			}
			);
    }      
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {return;}
  var dict = clay.getSettings(e.response);
	console.log(Clay.getItemsByMessageKey("MANUAL_STEPS"));
  Pebble.sendAppMessage(dict, function(e) {
    console.log('Config sent to Pebble successfully!');
  }, function(e) {
    console.log('Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});

// Listen for when an AppMessage is received
/*Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
		var dict = e.payload;
  	console.log('Got message: ' + JSON.stringify(dict));
  }                     
);*/

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    getWeather();
		//getConfig();
  }
);