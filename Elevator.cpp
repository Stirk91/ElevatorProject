#include "Elevator.h"

Elevator::Elevator(int x)
{
	set_name(x);
}

Elevator::Elevator(int x, const int number_of_floors, const int number_of_elevators)
{
	set_name(x);
	// sets the elevators initial level based on number of floors
	level = x * (number_of_floors / number_of_elevators); 
	
}

void Elevator::set_name(int x)
{
	name = x;
}

int Elevator::get_name()
{
	return name;
}

void Elevator::set_level(int x)
{

	level = x;
}

int Elevator::get_level()
{
	return level;
}

bool Elevator::moving_up()
{
	return (up == true);

}

void Elevator::set_direction(bool x)
{
	up = x;


}
bool Elevator::is_stationary()
{
	if (ele_up.getsize() == 0 && ele_down.getsize() == 0)
	{
		return true;
	}
	else
		return false;
}
void Elevator::up_stops()
{
	cout << "\t\t\t\t\t\t\t moving up" << endl;
	//set_level(ele_up.print());
	set_direction(true);
	ele_up.pop();

}
void Elevator::down_stops()
{
	cout << "\t\t\t\t\t\t\t moving down" << endl;
	//set_level (ele_down.print());
	set_direction(false);
	ele_down.pop();

}

bool Elevator::get_goodStatus() // NEW
{
	return goodStatus;
}

void Elevator::set_goodStatus(bool status) // NEW
{
	goodStatus = status;
}

void Elevator::set_distanceFromRequest(int floor_request_level) // NEW
{
	distanceFromRequest = abs(floor_request_level - level);
}

int Elevator::get_distanceFromRequest() // NEW
{
	return distanceFromRequest;
}
