class GeoCoordToDistance
{

private:
    static long double toRadians(long double degree);

public:
    constexpr static const long double RADIUS_MILES = 3956;
    static long double toMiles(long double lat1, long double long1, long double lat2, long double long2);
};
