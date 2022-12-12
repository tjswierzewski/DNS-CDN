#include <math.h>
#include "GeoCoordToDistance.h"

/**
 * Function to turn degrees to radians
 */
long double GeoCoordToDistance::toRadians(long double degree)
{
    long double radianPerDegree = (M_PI) / 180;
    return radianPerDegree * degree;
}
/**
 * Function to find the distance
 */
long double GeoCoordToDistance::toMiles(long double lat1, long double long1, long double lat2, long double long2)
{
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);

    long double dlat = lat2 - lat1;
    long double dlong = long2 - long1;

    long double distance = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2);

    distance = 2 * asin(sqrt(distance));
    return distance * RADIUS_MILES;
}