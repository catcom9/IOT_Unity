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

map.setView(cycleroute.getCenter(), 13);

L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
}).addTo(map);

for(var x = 0; x != 6; x++){
    L.marker(waypoints[x])
    .addTo(map)
    .bindPopup("Helllo");

}


