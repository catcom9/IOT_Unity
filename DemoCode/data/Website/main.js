var gateway = `ws://${window.location.hostname}/ws`;
var websocket;


window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
}

function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onmessage = onMessage;
}

function onMessage(evnet){
    var json_data = JSON.parse(event.data);
    console.log(json_data);
}
