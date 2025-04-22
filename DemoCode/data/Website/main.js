var gateway = `ws://192.168.1.34/ws`;
var websocket;
var cycleroute = [];
var polyline = L.polyline(cycleroute).addTo(map);;


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
        cycleroute.push(latlong)
        console.log(latlong);
        polyline.setLatLngs(cycleroute);
        L.marker(latlong).addTo(map);

    }
    console.log(json_data);
}

function GoHome(){
    map.setView([53.278189, -9.008778], 16);
}
