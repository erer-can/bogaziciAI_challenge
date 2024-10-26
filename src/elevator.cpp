using namespace std;
#include <vector>
#include <queue>


class Elevator
{
public:
    bool isMoving;
    vector<int> stops;
    int currentFloor;
    int passengers;
    bool isGoingUp;
    priority_queue< pair<int,int> > maxPQFloorTime;
    priority_queue< pair<int,int> > minPQFloorTime;
    priority_queue< pair<int,int> > MaxPQFloorEnergy;
    priority_queue< pair<int,int> > minPQFloorEnergy;

    Elevator()
        : currentFloor(0), isMoving(false), passengers(0), isGoingUp(false) {}


    Elevator(int startFloor)
        : currentFloor(startFloor), isMoving(false), passengers(0), isGoingUp(false) {}

    void addStop(int floor)
    {
        stops.push_back(floor);
        passengers++;
        isMoving = true;
    }

    int distanceToFloor(int floor, int currentFloor) const // destination and elevator floor
    {
        return abs(currentFloor - floor);
    }

    bool isIdle() const
    {
        return !isMoving && stops.empty();
    }

    int  timeUntilStop()
    {
        if (stops.empty())
        {
            return 0;
        }

        int nextStop = stops.front();
        return std::abs(currentFloor - nextStop);
    }

    int move()
    {
        if (stops.empty())
        {
            isMoving = false;
            return 0;
        }

        int nextStop = stops.front();
        if (currentFloor < nextStop)
        {
            currentFloor++;
        }
        else if (currentFloor > nextStop)
        {
            currentFloor--;
        }
        else
        {
            stops.erase(stops.begin());
            passengers--;
        }

        if (stops.empty())
        {
            isMoving = false;
        }

        return 1;
    }
};