// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;


// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};



// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: According to stl documentation size is
    // constant
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::unordered_map::clear clears elements
    // one by one
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: constant (insert) inside linear
    // (for) function
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find and insert are linear
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::vector::sort linearithmic, for loop
    // linear
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: std::vector::sort linearithmic, for loops
    // linear
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop linear
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find logarithmic, erase constant
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: unordered_map sort linearithmic
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find and insert linear
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-loop linear
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear worst case
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear worst case
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear worst case
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find linear worst case
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: unordered_map::find and while-loop linear
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_station(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    struct Station
    {
        StationID id_;
        Name name_;
        Coord coord_;
        std::vector<std::pair<Time,TrainID>> departures_ = {};
        RegionID region = NO_REGION;

    };

    using StationMap = std::map<StationID,Station*>;

    struct Region_node
    {
        RegionID id_ = NO_REGION;
        Name name_ = NO_NAME;
        std::vector<Coord> coords_;

        Region_node* parent_ = nullptr;
        std::vector<Region_node*> subregions_ = {};
    };

    std::map<StationID,Station*> station_map;

    std::unordered_map<RegionID, Region_node*> region_map;

};

#endif // DATASTRUCTURES_HH
