// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <string>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}



Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{
    for(auto const &pair : station_map){
        delete pair.second;
    }
    for(auto const &pair : region_map){
        delete pair.second;
    }
}

unsigned int Datastructures::station_count()
{
    return station_map.size();

}

void Datastructures::clear_all()
{
    station_map.clear();

}

std::vector<StationID> Datastructures::all_stations()
{

    std::vector<StationID> station_ids;
    for(auto const &pair : station_map){
        station_ids.insert(station_ids.begin(),pair.first);
    }
    return station_ids;

}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if(station_map.find(id)!=station_map.end()){return false;}

    Station* stat = new Station{id, name, xy};
    station_map.insert(std::make_pair(id, stat));

    return true;
}

Name Datastructures::get_station_name(StationID id)
{
    auto pair = station_map.find(id);
    if(pair==station_map.end()){return NO_NAME;}

    return pair->second->name_;

}

Coord Datastructures::get_station_coordinates(StationID id)
{
    auto pair = station_map.find(id);
    if(pair==station_map.end()){return NO_COORD;}

    return pair->second->coord_;
}

std::vector<StationID> Datastructures::stations_alphabetically()
{


    std::vector<StationID> station_ids;
    for(auto const &pair : station_map){
        station_ids.push_back(pair.first);
    }
    sort(station_ids.begin(),station_ids.end());

    return station_ids;

}

std::vector<StationID> Datastructures::stations_distance_increasing()
{


    std::vector<Station*> stations;
    std::vector<StationID> station_ids;
    for(auto const &pair:station_map){
        stations.insert(stations.begin(),pair.second);
    }
    sort(stations.begin(), stations.end(), [](Station* a, Station* b){
        double distance_a = sqrt(pow(a->coord_.x,2)+pow(a->coord_.y,2));
        double distance_b = sqrt(pow(b->coord_.x,2)+pow(b->coord_.y,2));
        if(distance_a==distance_b){return a->coord_.y<b->coord_.y;}
        return distance_a<distance_b;
    });
    for(Station* const &station:stations){
        station_ids.push_back(station->id_);
    }
    return station_ids;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{


    for(auto const &pair : station_map){
        if(pair.second->coord_ == xy){
            return pair.first;
        }
    }

    return NO_STATION;

}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto pair = station_map.find(id);
    if(pair==station_map.end()){return false;}
    pair->second->coord_ = newcoord;
    return true;
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{

    std::pair<Time,TrainID> departure = std::make_pair(time,trainid);
    auto stat = station_map.find(stationid);
    auto dep = find(stat->second->departures_.begin(),stat->second->departures_.end(),
                       departure);

    if(stat==station_map.end()){return false;}
    if(dep!=stat->second->departures_.end()){return false;}
    stat->second->departures_.push_back(departure);
    return true;

}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    std::pair<Time,TrainID> departure = std::make_pair(time,trainid);
    auto stat = station_map.find(stationid);
    auto dep = find(stat->second->departures_.begin(),stat->second->departures_.end(),
                       departure);
    if(stat==station_map.end()){return false;}
    if(dep==stat->second->departures_.end()){return false;}
    stat->second->departures_.erase(dep);
    return true;
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{

    auto stat = station_map.find(stationid);
    std::vector<std::pair<Time, TrainID>> departures;
    if(stat==station_map.end()){
        departures.push_back(std::make_pair(NO_TIME, NO_TRAIN));
        return departures;
    }
    for(auto& departure:stat->second->departures_){
        if(departure.first>=time){
            departures.push_back(departure);
        }
    }
    sort(departures.begin(),departures.end(),[](std::vector<std::pair<Time,TrainID>>::
         value_type pair_a, std::vector<std::pair<Time,TrainID>>::value_type &pair_b){
        if(pair_a.first==pair_b.first){
            return pair_a.second<pair_b.second;
        }
        return pair_a.first<pair_b.first;
    });
    return departures;

}

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto reg = region_map.find(id);
    if(reg!=region_map.end()){return false;}
    Region_node* region = new Region_node{id, name, coords};
    region_map.insert(std::make_pair(id, region));
    return true;
}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> region_ids;
    for(auto const& region:region_map){
        region_ids.push_back(region.second->id_);
    }
    return region_ids;
}

Name Datastructures::get_region_name(RegionID id)
{
    auto reg = region_map.find(id);
    if(reg == region_map.end()){return NO_NAME;}
    return reg->second->name_;
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto reg = region_map.find(id);
    if(reg == region_map.end()){return std::vector<Coord>{NO_COORD};}
    return reg->second->coords_;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto subreg = region_map.find(id);
    auto parent_reg = region_map.find(parentid);

    if(subreg==region_map.end()){return false;}
    if(parent_reg==region_map.end()){return false;}
    else if(subreg->second->parent_!=nullptr){return false;}

    subreg->second->parent_ = parent_reg->second;
    parent_reg->second->subregions_.push_back(subreg->second);
    return true;
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    auto stat = station_map.find(id);
    auto reg = region_map.find(parentid);

    if(stat==station_map.end()){return false;}
    if(reg==region_map.end()){return false;}
    else if(stat->second->region!=NO_REGION){return false;}

    stat->second->region=parentid;
    return true;

}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    auto stat = station_map.find(id);
    if(stat==station_map.end()){return std::vector<RegionID>{NO_REGION};}
    if(stat->second->region==NO_REGION){return std::vector<RegionID>{NO_REGION};}

    std::vector<RegionID> regions;
    auto reg = region_map.find(stat->second->region);
    Region_node* region = reg->second;
    while(region->parent_!=nullptr){
        regions.push_back(region->id_);
        region = region->parent_;
    }
    regions.push_back(region->id_);
    return regions;
}



std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{

    throw NotImplemented("all_subregions_of_region()");
    /*auto region_pair = region_map.find(id);
    std::vector<RegionID> region_ids;
    for(Region_node* region : region_pair->second->subregions_){

        region_ids.insert(region_ids.begin(),region->id_);

    }*/
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}
