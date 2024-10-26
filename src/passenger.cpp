#include <iostream>
#include <vector>
#include <queue>
#include "elevator.cpp"
using namespace std;

class Passenger
{
private:
    int currentFloor;
    int destinationFloor;
    bool isInsideElevator;
    int cWT; //constant of waiting times
    int cEC; //constant of energies consumed
    vector<int> waitingTimes;
    vector<int> consumedEnergies;
    vector< pair<int,int> > summedUpWeights;

public:

    Passenger(int currentFloor, int destinationFloor)
        : currentFloor(currentFloor), destinationFloor(destinationFloor), isInsideElevator(false) {}

    void setConstants(int cWT, int cEC)
    {
        this->cWT = cWT;
        this->cEC = cEC;
    }

    pair<Elevator, int> calculateWaitingTimeAndEnergy(vector<Elevator> elevators)
    {
        int waitingTime;
        int energy;
        int index ;
        Elevator bestElevator;
        for (int i = 0; i < elevators.size(); ++i)
        {
            waitingTime = calculateWaitingTimeSingle(elevators[i]);
            waitingTimes.push_back(waitingTime);
            //TODO perform the tests to optimize constants
            energy = calculateEnergySingle(elevators[i]);
            consumedEnergies.push_back(energy);
        }
        bestElevator = findBestElevator(elevators);
        return make_pair(bestElevator, index);
    }
    
    int calculateEnergySingle(Elevator elevator)
    {
        int energy;
        int top;
        int bot;
        int ind;
        if (elevator.isIdle())
        {
            energy = elevator.distanceToFloor(currentFloor, elevator.currentFloor);
            return energy;
        }
        else
        {
            if (elevator.isGoingUp)
            {
                top = elevator.MaxPQFloorEnergy.top().first;
                if (currentFloor <= top)
                {
                    energy = (top - currentFloor) + abs(top - destinationFloor);
                    return energy;
                } else
                {
                    energy = abs(top - destinationFloor);
                    return energy;
                }
                
            
            } else
            {
                bot = -elevator.minPQFloorEnergy.top().first;
                if (currentFloor >= bot)
                {
                    energy = (currentFloor - bot) + abs(bot - destinationFloor);
                    return energy;
                } else
                {
                    energy = abs(bot - destinationFloor);
                    return energy;
                }

            }

        }
        return energy;
    }

    int calculateWaitingTimeSingle(Elevator elevator)
    {
        int waitingTime;
        int top;
        int bot;
        int ind;

        if (elevator.isIdle())
        {
            waitingTime = elevator.distanceToFloor(currentFloor, elevator.currentFloor) + this->timeInElevator(currentFloor, destinationFloor);
            return waitingTime;
        }
        else
        {
            if (elevator.isGoingUp)
            {
                pair<int, int> topEle = elevator.maxPQFloorTime.top();
                top = topEle.first;
                ind = topEle.second;
                if (top <= currentFloor)
                {
                    waitingTime =  currentFloor - elevator.currentFloor + this->timeInElevator(currentFloor, destinationFloor); ;
                    return waitingTime;
                } else
                {
                    waitingTime = elevator.distanceToFloor(top, elevator.currentFloor + this->timeInElevator(elevator.currentFloor, destinationFloor));
                    return waitingTime;
                }
            } else
            {
                pair<int, int> botEle = elevator.minPQFloorTime.top();
                bot = -botEle.first;
                ind = botEle.second;
                if (bot>= currentFloor)
                {
                    waitingTime = currentFloor - bot + this->timeInElevator(currentFloor,destinationFloor);
                    return waitingTime;
                }else
                {
                    waitingTime = elevator.distanceToFloor(bot, elevator.currentFloor + this->timeInElevator(bot, destinationFloor));
                    return waitingTime;
                }
            }
            
        }

        return waitingTime;
    }

    int timeInElevator(int currentFloor, int destinationFloor)
    {
        int time;
        time = abs(destinationFloor - currentFloor);
        return time;
    }

    Elevator findBestElevator(vector<Elevator> elevators)
    {
        Elevator bestElevator;
        int sum;

        for (int i = 0; i < elevators.size(); ++i)
        {
            sum = cWT * waitingTimes[i] + cEC * consumedEnergies[i];
            summedUpWeights.push_back(make_pair(sum, i));
        }
        auto minPair = *std::min_element(summedUpWeights.begin(), summedUpWeights.end(),[](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.first < b.first;
        });
        bestElevator = elevators[minPair.second];
        return bestElevator;
    }
};