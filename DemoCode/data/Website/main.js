var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var polyline_created = 0;
var cycleroute

window.addEventListener('load', onLoad);


function onLoad(event) {
    initWebSocket();
}

function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onmessage = onMessage;
}

function onMessage(event){
    var json_data = JSON.parse(event.data);
    if(json_data.fix == 1){
        var latlong = [json_data.latitude, json_data.longitude];
        if(polyline_created == 0){
            cycleroute = L.polyline(latlong).addTo(map);
            L.marker(latlong).addTo(map);
            polyline_created = 1;
        } else {
            cycleroute.addLatLng(latlong);
            L.marker(latlong).addTo(map);
            console.log("Added point");
        }
    }
    console.log(json_data);
}

function GoHome(){
    map.setView([53.278189, -9.008778], 16);
}