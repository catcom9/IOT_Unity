var waypoints = [
    [53.278189, -9.008778],
    [53.278000, -9.008789],
    [53.277846, -9.008902],
    [53.277633, -9.009052],
    [53.277380, -9.009162],
    [53.277238, -9.009913],
]

var map = L.map('MainMap').setView([51.505, -0.09], 13);

var cycleroute = L.polyline(waypoints, {color: 'red'}).addTo(map);
cycleroute.on("mouseover", hoverNear)


var tooltip = L.tooltip()
    .setLatLng(waypoints[0])
    .setContent('Hello world!<br />This is a nice tooltip.')
    .addTo(map);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);


function hoverNear(e){
    console.log("Hiya");
}
