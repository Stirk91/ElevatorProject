#include "ElevatorOperations.h"
#include "Elevator.h"
#include "Utilities.h"
#include "User.h"


/*Main function for calucation elevator operations For users logic and elevator movement*/
void elevator_op(vector<Elevator*> &v, const int& number_of_floors, int& total_users, int& current_time, vector<int>& waiting_times)
{
	vector<Elevator*>::iterator el;

	for (el = v.begin(); el != v.end(); ++el)
	{
		if (DEBUG == YES)
		{
			cout << "\t\t Elevator " << (*el)->get_name() << endl;
		};

		if ((*el)->get_level() == 0) {
			(*el)->set_direction(UP);
		}
		else if ((*el)->get_level() == number_of_floors) {
			(*el)->set_direction(DOWN);
		}
	



// USER BOARDING LOGIC //

		// if the elevator is stationary and has no floor requests
		if ((*el)->isBoarding() == true)
		{
			if (DEBUG == YES)
			{
				cout << endl;
				cout << "***ELEVATOR Finished Boarding***" << endl;
				cout << endl;
			}
			(*el)->setBoarding(false);
			(*el)->last_floor_down = { -1, -1, -1 };
			(*el)->last_floor_up = { -1, -1, -1 };
		}

		// if the elevator is moving DOWN, and a floor request matches the current floor
		else if (((*el)->last_floor_down[DIRECTION] != -1) && (
			
			( ((*el)->moving_up() == NO) && ((*el)->last_floor_down[FLOOR] == (*el)->get_level()) && (*el)->ele_down.isempty() == NO )
			
			// OR if the elevator's UP queue is empty and it's DOWN queue is not empty, and a floor request matches the current floor
			|| ( ( ((*el)->ele_up.isempty() == YES) && (*el)->ele_down.isempty() == NO ) && ((*el)->last_floor_up[FLOOR] == (*el)->get_level()) )

			// OR if the elevator is now stationary, but it had a DOWN request, and a floor request matches the current floor
			|| ( ((*el)->is_stationary() == YES) && ((*el)->last_floor_down[FLOOR] == (*el)->get_level()) ) ) )

		{
			if (DEBUG == YES)
			{
				cout << endl;
				cout << "***Elevator Starts Boarding***" << endl;
				cout << "Elevator: " << (*el)->get_name() << " stops at " << (*el)->get_level() << endl;
				cout << "Request floor: " << (*el)->last_floor_down[FLOOR] << endl;
				cout << "Request direction: " << (*el)->last_floor_down[DIRECTION] << endl;
				cout << "Request time: " << (*el)->last_floor_down[TIME] << endl;
				cout << "Current time: " << current_time << endl;
				cout << "Waiting time: " << current_time - (*el)->last_floor_down[TIME] << endl;
				cout << endl;
			}
		


			vector<User*> users = generateUsers((*el)->get_level(), (*el)->last_floor_down[DIRECTION], number_of_floors, total_users);
			printUserStatus(users);


			for (int index = 0; index < users.size(); index++)
			{
				// vector for each users { FLOOR, DIRECTION, which matches the elevator's floor request direction, and the current time }
				vector<int> eachUserRequest = { users[index]->get_userDestination(), (*el)->last_floor_down[DIRECTION], current_time };

				(*el)->ele_down.push_down(eachUserRequest);

				waiting_times.push_back(current_time - (*el)->last_floor_down[TIME]); // add waiting time to vector


				if (DEBUG == YES)
				{
					cout << "User " << users[index]->get_UserID() << " entered Elevator " << (*el)->get_name() << endl;
				}
			}

			// Delete users vector (dynamic memory)
			deleteVector(users);

			//(*el)->ele_down.pop_front(); // removes request from the queue

			(*el)->setBoarding(true);
		}

		// if the elevator is moving UP and a floor request matches the current floor
		else if (((*el)->last_floor_up[DIRECTION] != -1) && (
			
			(((*el)->moving_up() == YES) && ((*el)->last_floor_up[FLOOR] == (*el)->get_level())  )

			|| // OR  if the elevator's DOWN queue is empty and it's UP queue is not empty, and a floor request matches the current floor
			((((*el)->ele_down.isempty() == YES) && (*el)->ele_up.isempty() == NO) && ((*el)->last_floor_up[FLOOR] == (*el)->get_level()) )

			|| // OR  if the elevator is now stationary, but it had an UP request 
			(((*el)->is_stationary() == YES) && ((*el)->last_floor_up[FLOOR] == (*el)->get_level())) ) )

		{
			if (DEBUG == YES)
			{
				cout << endl;
				cout << "***Elevator Starts Boarding***" << endl;
				cout << "Elevator: " << (*el)->get_name() << " stops at " << (*el)->get_level() << endl;
				cout << "Request floor: " << (*el)->last_floor_up[FLOOR] << endl;
				cout << "Request direction: " << (*el)->last_floor_up[DIRECTION] << endl;
				cout << "Request time: " << (*el)->last_floor_up[TIME] << endl;
				cout << "Current time: " << current_time << endl;
				cout << "Waiting time: " << current_time - (*el)->last_floor_up[TIME] << endl;
				cout << endl;
			}



			vector<User*> users = generateUsers((*el)->get_level(), (*el)->last_floor_up[DIRECTION], number_of_floors, total_users);
			printUserStatus(users);


			for (int index = 0; index < users.size(); index++)
			{
				// vector for each users { FLOOR, DIRECTION, which matches the elevator's floor request direction, and the current time }
				vector<int> eachUserRequest = { users[index]->get_userDestination(), (*el)->last_floor_up[DIRECTION], current_time };

				(*el)->ele_up.push_up(eachUserRequest);

				waiting_times.push_back(current_time - (*el)->last_floor_up[TIME]); // add waiting time to vector

				if (DEBUG == YES)
				{
					cout << "User " << users[index]->get_UserID() << " entered Elevator " << (*el)->get_name() << endl;
				}
			}

			(*el)->ele_up.pop_front(); // removes request from the queue

			// Delete users vector (dynamic memory)
			deleteVector(users);

			(*el)->setBoarding(true);
		}

		
// ELEVATOR MOVEMENT LOGIC //

//FORCE ELEVATOR OFF LOOPS ON TOP OR BOTTOM FLOOR CAUSING PROGRAM TO CRASH
		else if ((*el)->get_level() == number_of_floors) {
			(*el)->ele_up.pop();
			(*el)->set_direction(DOWN);
			(*el)->down_stops();

		}
		else if ((*el)->get_level() == 0) {
			(*el)->ele_down.pop();
			(*el)->set_direction(UP);
			(*el)->up_stops();

		}
		else if ((*el)->ele_down.getsize() == 0 && (*el)->ele_up.getsize() == 0) {
			
			if (DEBUG == YES)
			{
				cout << "\t\t Elevator " << (*el)->get_name() << "\t\t idle" << endl;
			}
		}
		else if ((*el)->ele_down.getsize() != 0 && (*el)->ele_up.getsize() == 0) {
			(*el)->down_stops();
			
			if (DEBUG == YES)
			{
				cout << "\t\t Elevator " << (*el)->get_name() << "\t\t on floor " << (*el)->get_level() << endl;
			}
		}
		else if ((*el)->ele_down.getsize() == 0 && (*el)->ele_up.getsize() != 0) {
			(*el)->up_stops();
			
			if (DEBUG == YES)
			{
				cout << "\t\t Elevator " << (*el)->get_name() << "\t\t on floor " << (*el)->get_level() << endl;
			}
		}
		else if ((*el)->ele_up.getsize() != 0 && (*el)->moving_up() == true) {
			(*el)->up_stops();

			if (DEBUG == YES)
			{
				cout << "\t\t Elevator " << (*el)->get_name() << "\t\t on floor " << (*el)->get_level() << endl;
			}
		}
		else if ((*el)->ele_down.getsize() != 0 && (*el)->moving_up() == false) {
			(*el)->down_stops();

			if (DEBUG == YES)
			{
				cout << "\t\t Elevator " << (*el)->get_name() << "\t\t on floor " << (*el)->get_level() << endl;
			}
		}
		else if ((*el)->ele_up.getsize() != 0 && (*el)->moving_up() == false) {
			(*el)->set_direction(true);
		}
		else if ((*el)->ele_down.getsize() != 0 && (*el)->moving_up() == true) {
			(*el)->set_direction(false);
		}
		else { 
			if (DEBUG == YES)
			{
				cout << "Nothing happend " << endl;
			}
		}
	}
}

// Calculates elevators' distances from requested floor
void calculateDistance(vector<Elevator*>& elevators, int task)
{
	
	for (int index = 0; index < elevators.size(); index++)
	{
		elevators[index]->set_distanceFromRequest(task);

		if (DEBUG == YES)
		{
			cout << "Elevator " << index << " distance from request: " <<
				elevators[index]->get_distanceFromRequest() << endl;
		}
	}
}