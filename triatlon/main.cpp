#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <map>
#include "triatlon.h"


int main()
{
    srand(time(NULL));

    int numTeams = 300;
    int athletesPerTeam = 3;
    Race race(numTeams, athletesPerTeam);

    std::thread raceThread(&Race::simulateRace, &race);
    raceThread.join();

    return 0;
}
