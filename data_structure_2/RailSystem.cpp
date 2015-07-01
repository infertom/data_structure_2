#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include "RailSystem.h"
//#define DEBUG

void RailSystem::reset(void) {

    // TODO: reset the data objects of the 
    // City objects' contained in cities
	for ( std::map<string, City*>::iterator it = cities.begin(); it != cities.end(); it++){
		it->second = new City(it->first);
		it->second->total_distance = INT_MAX;
		it->second->total_fee = INT_MAX;
	}
    
}

RailSystem::RailSystem(string const &filename) {
    
    load_services(filename);
}

void RailSystem::load_services(string const &filename) {

	ifstream inf(filename.c_str());
	string from, to;
	int fee, distance;
#ifdef DEBUG
	cout<<"xxx"<<endl;
	if ( inf.bad() ) cout<<"bad"<<endl;
	else if ( inf.fail() ) cout<<"fail"<<endl;
	else if ( inf.eof() ) cout<<"eof"<<endl;
	else cout<<"good"<<endl;
#endif

	while ( inf.good() ) {
#ifdef DEBUG
		cout<<111<<endl;
#endif
		// Read in the from city, to city, the fee, and distance.
		inf >> from >> to >> fee >> distance;

		if ( inf.good() ) {
		
			// TODO: Add entries in the cities container and
			// and services in the rail system for the new 
            // cities we read in.
			if ( !cities[from] ) cities[from] = new City(from);
			if ( !cities[to] ) cities[to] = new City(to);
			outgoing_services[from].push_back(new Service(to, fee, distance));

		}
	}

	inf.close();
}

RailSystem::~RailSystem(void) {

    // TODO: release all the City* and Service*
    // from cities and outgoing_services
	for ( std::map<string, City*>::iterator it = cities.begin(); it != cities.end(); it++){
		delete it->second;
	}
	for ( std::map<string, list<Service*>>::iterator it = outgoing_services.begin(); it != outgoing_services.end(); it++){
		for ( std::list<Service*>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++){
			delete *it2;
		}
	}
}

void RailSystem::output_cheapest_route(const string& from,
                const string& to, ostream& out) {

	reset();
	pair<int, int> totals = calc_route(from, to);

	if (totals.first == INT_MAX) {
		out << "There is no route from " << from << " to " << to << "\n";
	} else {
		out << "The cheapest route from " << from << " to " << to << "\n";
		out << "costs " << totals.first << " euros and spans " << totals.second
			<< " kilometers\n";
        cout << recover_route(to) << "\n\n";
	}
}

bool RailSystem::is_valid_city(const string& name) {

	return cities.count(name) == 1;
}

pair<int, int> RailSystem::calc_route(string from, string to) {

	priority_queue<City*, vector<City*>, Cheapest> candidates;

    // TODO: Implement Dijkstra's shortest path algorithm to
    // find the cheapest route between the cities
	City * currentCity = cities[from];

	currentCity->from_city = from;
	currentCity->total_distance = 0;
	currentCity->total_fee = 0;
	candidates.push(currentCity);
#ifdef DEBUG
	cout<<"xxx1:"<<cities[from]->name<<"  "<<cities[from]->from_city<<endl;
#endif
	
	//Çó×î¶ÌÂ·
	while ( !candidates.empty() )
	{
		currentCity = candidates.top();
		candidates.pop();
		
		if ( currentCity->visited ) continue;
		else currentCity->visited = true;
		if ( currentCity->name == to ) break;
 		for ( std::list<Service*>::iterator it = outgoing_services[currentCity->name].begin(); it != outgoing_services[currentCity->name].end(); it++){
 			if ( !cities[(*it)->destination]->visited && cities[(*it)->destination]->total_fee > currentCity->total_fee + (*it)->fee ){
 				cities[(*it)->destination]->total_fee = currentCity->total_fee + (*it)->fee;
 				cities[(*it)->destination]->total_distance = currentCity->total_distance + (*it)->distance;
				cities[(*it)->destination]->from_city = currentCity->name;
				candidates.push(cities[(*it)->destination]);
 			}
 		}
	}
#ifdef DEBUG
	cout<<"xxx2:"<<cities[from]->name<<"  "<<cities[from]->from_city<<endl;
#endif
    

	// Return the total fee and total distance.
	// Return (INT_MAX, INT_MAX) if not path is found.
	if (cities[to]->visited) {
		return pair<int,int>(cities[to]->total_fee, cities[to]->total_distance);
	} else {
		return pair<int,int>(INT_MAX, INT_MAX);
 	}

}

string RailSystem::recover_route(const string& city) {
	
	// TODO: walk backwards through the cities
	// container to recover the route we found
	string ans = city;
	City * currentCity = cities[city];
	while ( currentCity->name != currentCity->from_city )
	{
#ifdef DEBUG
		cout<<currentCity->name<<"   "<<currentCity->from_city<<endl;
		getchar();
#endif
		ans = currentCity->from_city + " to " + ans;
		currentCity = cities[currentCity->from_city];
	}
	
	return ans;
}
