import requests
from math import cos, sin, atan2, pi, sqrt


def degToRad(deg):
    return deg/360*2*pi


def distance(u, v):
    if type(u) is tuple and type(v) is tuple :
        r_lat1 = float(u[0])
        r_lon1 = float(u[1])
        r_lat2 = float(v[0])
        r_lon2 = float(v[1])
    else:
        r_lat1 = float(u.get("lat"))
        r_lon1 = float(u.get("lon"))
        r_lat2 = float(v.get("lat"))
        r_lon2 = float(v.get("lon"))

    R = 6371000
    lat1 = degToRad(r_lat1) / 2
    lat2 = degToRad(r_lat2) / 2
    lon1 = degToRad(r_lon1) / 2
    lon2 = degToRad(r_lon2) / 2

    dlat = lat2-lat1
    dlon = lon2-lon1
    a = (pow(sin(dlat)/2, 2)
         + cos(lat1)*cos(lat2)*pow(sin(dlon)/2, 2))
    c = 2*atan2(sqrt(a), sqrt(1-a))
    dist = R*c
    return dist


def nearestRoad(latitude, longitude, radius):
    ''' Returns the 2 closest points from the coordinates given.'''
    req = '''<osm-script output="json">
  <query type="way">
    <around lat="{lat}" lon="{long}" radius="{rad}"/>
    <has-kv k="highway" modv="" v=""/>
  </query>
  <union>
    <item/>
  </union>
  <print/>
</osm-script>'''.format(lat=latitude, long=longitude, rad=radius)
    headers = {'content-type': 'application/xml'}
    r = requests.post("http://overpass-api.de/api/interpreter",
                      headers=headers, data=req)
    assert (r.status_code == 200)

    ways = r.json()["elements"]
    nodes = []
    for way in ways:
        nodes += way['nodes']

    return nodes


def nearestTransport(latitude, longitude, radius):
    req = '''
<osm-script output="json" timeout="25">
  <union into="_">
    <query into="_" type="node">
      <has-kv k="highway" modv="" v="bus_stop"/>
      <around from="_" into="_" lat="{lat}" lon="{lon}" radius="{rad}"/>
    </query>
    <query into="_" type="node">
      <has-kv k="railway" modv="" v="subway_entrance"/>
      <around from="_" into="_" lat="{lat}" lon="{lon}" radius="{rad}"/>
    </query>
  </union>
  <print e="" from="_" geometry="skeleton" limit="" mode="body" n="" order="id" s="" w=""/>
  <recurse from="_" into="_" type="down"/>
  <print e="" from="_" geometry="skeleton" limit="" mode="skeleton" n="" order="quadtile" s="" w=""/>
</osm-script>'''.format(lat=latitude, lon=longitude, rad=radius)

    headers = {'content-type': 'application/xml'}
    r = requests.post("http://overpass-api.de/api/interpreter",
                      headers=headers, data=req)

    assert (r.status_code == 200)
    json = r.json()["elements"]

    stations = dict()
    for station in json:
        d = distance((latitude, longitude),
                     (station['lat'], station['lon']))
        stations[d] = station['id']

    keys = list(stations.keys())
    keys.sort()

    return [stations[k] for k in keys[:2]]


def coordinates(adress):
    url = "http://nominatim.openstreetmap.org/search?q={}&format=json".format("+".join(adress.split()))
    print("requesting", url)
    r = requests.get(url)
    assert (r.status_code == 200)
    json = r.json()[0]
    name = json["display_name"]
    lat = json["lat"]
    lon = json["lon"]

    print("Found {} at ({},{})".format(name, lat, lon))
    return (lat, lon)
