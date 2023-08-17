#include "simulator.hpp"
#include <iostream>

Trajectory::Trajectory(cartVal pos){
	lenght = -1;
	this->add_point(pos);
	return;
}

void Trajectory::add_point(cartVal pos){
	if(this->lenght == 99){ //lenght should not be more than 100 points
		for(int i = 0; i < 98; i++){
			this->Points[i] = this->Points[i+1];
		}
	}
	else {
		this->lenght ++;
	}	
	this->Points[this->lenght] = pos;
	return;
}

int Trajectory::get_lenght(){
	return this->lenght;
}

cartVal Trajectory::getPoint(int index){
	return this->Points[index];
}

int main(){
	cartVal pos = {10,50};
	Trajectory traj = Trajectory(pos);
	cartVal posinit = traj.getPoint(0);
	std::cout << posinit.x << " , " << posinit.y << std::endl;
	return 0;
}
