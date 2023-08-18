#include "simulator.hpp"
#include <iostream>
//---------------------------------------- Trajectory ---------------------------------
Trajectory::Trajectory(cartVal pos){
	lenght = -1;
	this->add_point(pos);
	return;
}

void Trajectory::add_point(cartVal pos){
	if(this->lenght == 99){ //lenght should not be more than 100 points
		for(int i = 0; i < 99; i++){
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
//------------------------------------------------ MoovObj ----------------------------
MoovObj::MoovObj(){
	position = {0,0};
	speed = {0,0};
	accel = {0,0};
	return;
}

void MoovObj::change_acc(cartVal acc){
	accel = acc;
	return;
}

void MoovObj::actualize(){
	speed.x = accel.x * deltaT + speed.x;
	speed.y = accel.y * deltaT + speed.y;
	position.x = speed.x * deltaT + position.x;
	position.y = speed.y * deltaT + position.y;
	return;
}

cartVal MoovObj::get_pos(){
	return position;
}

cartVal MoovObj::get_speed(){
	return speed;
}

cartVal MoovObj::get_accel(){
	return accel;
}

//---------------------------------------------- IMU ---------------------------------
IMU::IMU(float moy, float covar){
	mean = moy;
	cov = covar;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator = std::default_random_engine(seed);
	distribution = std::normal_distribution<float>(mean, cov);
	return;
}

IMU::IMU(){
	mean = 0;
	cov = 0;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator = std::default_random_engine(seed);
        distribution = std::normal_distribution<float>(mean, cov);

	return;
}

float IMU::getMean(){
	return mean;
}

float IMU::getCov(){
	return cov;
}

cartVal IMU::calculusVelocity(cartVal trueAccel){
	float acc_noise_x = distribution(generator);
	float acc_noise_y = distribution(generator);
	cartVal acc_noisy = {trueAccel.x + acc_noise_x, trueAccel.y + acc_noise_y};
	return acc_noisy;
}
//---------------------------------------------- Drone -------------------------------
Drone::Drone(float mean, float cov){
	imu = IMU(mean, cov);
	MoovObj();
}

cartVal Drone::getACC(){
	return imu.calculusVelocity(get_accel());
}
//---------------------------------------------- Test --------------------------------

int main(){
	/*cartVal pos = {10,50};
	Trajectory traj = Trajectory(pos);
	cartVal posinit = traj.getPoint(0);
	std::cout << posinit.x << " , " << posinit.y << std::endl;
	for (int j=0; j < 151; j++){
		cartVal posi = {j,j+2};
		traj.add_point(posi);
	}
	for (int i=0; i<100; i++){
		std::cout << i << ": " << (traj.getPoint(i)).x << " , " << (traj.getPoint(i)).y << std::endl;
	} */
	/*
	MoovObj obj;
	cartVal acc = {5,2};
	obj.change_acc(acc);
	for (int i=0; i<15; i++){
		obj.actualize();
		std::cout << i << ": " << obj.get_pos().x << "," << obj.get_pos().y << std::endl;
	}
	*/
	/*
	IMU imu(0.0, 0.2);
	cartVal acc = {4,5};
	for (int k=0; k<10; k++){
		std::cout << k << ": " << imu.calculusVelocity(acc).x << "," << imu.calculusVelocity(acc).y << std::endl;
	}
	*/
	Drone drn(0.0,0.02);
	cartVal acc_init = {2.0,2.0};
	drn.change_acc(acc_init);
	cartVal imu_pose = {0,0};
	cartVal imu_speed = {0,0};
	for(int it=0; it<100; it++){
		drn.actualize();
		std::cout << it << ": real pose : " << drn.get_pos().x << "," << drn.get_pos().y << std::endl;
		std::cout << it << ": imu acc : " << drn.getACC().x << "," << drn.getACC().y << std::endl;
		imu_speed.x = imu_speed.x + drn.getACC().x * deltaT;
		imu_speed.y = imu_speed.y + drn.getACC().y * deltaT;
                imu_pose.x = imu_pose.x + imu_speed.x * deltaT;
                imu_pose.y = imu_pose.y + imu_speed.y * deltaT;
                std::cout << it << ": imu pose : " << imu_pose.x << "," << imu_pose.y << std::endl;
		std::cout << "---------------------------------------------" << std::endl;
	}
	return 0;
}
