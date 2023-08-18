#pragma once
#include <chrono>
#include <random>

#define deltaT 1

//cartesian ref 2D
typedef struct {
	float x;
	float y;
} cartVal;

class Trajectory{
	public:
		Trajectory(cartVal pos);
		cartVal getPoint(int index);
		void add_point(cartVal pos);
		int get_lenght();

	private:
		cartVal Points[100];
		int lenght;
};

class MoovObj {
	public: 
		MoovObj();
		void change_acc(cartVal acc);
		void actualize();
		cartVal get_pos();
		cartVal get_speed();
		cartVal get_accel();


	private: 
		cartVal position;
		cartVal speed;
		cartVal accel;
};


class IMU { //model of white gaussian noise
	public: 
		IMU(float mean, float cov);
		IMU();
		float getMean();
		float getCov();
		cartVal calculusVelocity(cartVal trueAccel);
	private:
		float mean; // mu noise model
		float cov; //s2 noise model 
		std::default_random_engine generator;
		std::normal_distribution<float> distribution;
};

class Drone: public MoovObj{
	public:
		Drone(float mean, float cov);
		cartVal getACC();
	private:
		IMU imu;
};

class Map{
	public:
		Map();
		void actualize();
		void show();
		Drone get_drone();
		Trajectory get_traj();
	private:
		Drone drn;
		Trajectory traj;

};
