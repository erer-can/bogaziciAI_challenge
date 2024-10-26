#include <iostream>
#include <vector>
#include "elevator.cpp"
#include "passenger.cpp"
using namespace std;

class ElevatorSystem
{
private:
    vector<Elevator> elevators;
    int totalEnergyConsumed;
    int totalWaitingTime;
    int numVisitors;

public:
    ElevatorSystem(int numElevators, int startingFloor)
        : totalEnergyConsumed(0), totalWaitingTime(0), numVisitors(0)
    {
        for (int i = 0; i < numElevators; ++i)
        {
            elevators.emplace_back(startingFloor);
        }
    }

    void newCall(int from, int to, Passenger pass)
    {
        auto [bestElevator,bestElevatorIndex] = pass.calculateWaitingTimeAndEnergy(elevators);
        bestElevator.maxPQFloorTime.push(make_pair(to,0)); // 0 should be index of that elevator
        bestElevator.minPQFloorTime.push(make_pair(-to,0)); // 0 should be index of that elevator
        numVisitors++;

        Passenger passenger(from, to);

        int minDistance = numeric_limits<int>::max();

        if (bestElevatorIndex != -1)
        {
            Elevator &elevator = elevators[bestElevatorIndex];
            elevator.addStop(from);
            elevator.addStop(to);
            numVisitors++;
        }
    }

    void updateElevators()
    {
        for (auto &elevator : elevators)
        {
            int move = elevator.move();
            totalEnergyConsumed += move;
            totalWaitingTime += move * elevator.passengers;
        }
    }
    pair<int,int> findBestConstants()
    {
        vector<pair<int, int>> results;
        int randomFrom = rand() % 10 + 1;
        int randomTo = rand() % 10 + 1;

        Passenger passenger(randomFrom,randomTo);
        for (int i = 0; i < 100; ++i) // Example: run 100 simulations with random constants
        {
            int cWT = rand() % 10 + 1; // Random constant for waiting time
            int cEC = rand() % 10 + 1; // Random constant for energy consumption
            passenger.setConstants(cWT, cEC);
            runSimulation(10, 0.1, 1000, passenger); // Example: run simulation with 10 floors, 0.1 visitor frequency, and 1000 time units
            results.push_back(make_pair(cWT, cEC));
        }
        sort(results.begin(), results.end(), [](const pair<int, int> &a, const pair<int, int> &b) {
            return a.first + a.second < b.first + b.second; // Example: sort by sum of constants
        });
        return results.front();
    }

    void runSimulation(int numFloors, double visitorFrequency, int totalTime, Passenger passenger)
    {
        srand(std::time(0));
        double currentTime = 0;
        while (currentTime < totalTime)
        {
            if (rand() % 100 < (visitorFrequency * 100))
            {
                int from = std::rand() % numFloors;
                int to = std::rand() % numFloors;
                if (from != to)
                {
                    newCall(from, to, passenger);
                }
            }
            updateElevators();
            currentTime++;
        }
        printStatistics();
    }

    void printStatistics() const
    {
        cout << "\nSimulation Results:\n";
        cout << "Total Energy Consumed: " << totalEnergyConsumed << " units\n";
        cout << "Average Waiting Time: " << (numVisitors > 0 ? totalWaitingTime / numVisitors : 0) << " seconds\n";
    }
};