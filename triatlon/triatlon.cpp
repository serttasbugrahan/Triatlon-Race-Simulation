#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>
#include <algorithm>
#include <map>
#include "triatlon.h"

Race::Race(int numTeams, int athletesPerTeam)
{
    for (int i = 0; i < numTeams; ++i)
    {
        for (int j = 0; j < athletesPerTeam; ++j)
        {
            float speed = (rand() % 400 + 100) / 100.0f;
            athletes.emplace_back(j + 1, i + 1, speed);
        }
    }
}

void Race::simulateRace()
{
    int completionOrder = 1;
    int firstFinishTeam = 0;
    int firstFinishAthleteId = 0;

    int numAthletes;
    std::cout << "Enter the number of athletes to get info: ";
    std::cin >> numAthletes;
    std::vector<std::pair<int, int>> athleteRequests;
    for (int i = 0; i < numAthletes; ++i)
    {
        int teamId, athleteId;
        std::cout << "Enter team id and athlete id (example: team 2, athlete 3): ";
        std::cin >> teamId >> athleteId;
        athleteRequests.emplace_back(teamId, athleteId);
    }

    while (!allRaceFinished())
    {
        for (Athlete &athlete : athletes)
        {
            float distance = athlete.getSpeed();
            athlete.move(distance);
            if (athlete.getPosition() >= 55000.0f && athlete.getCompletionOrder() == 0)
            {
                athlete.setCompletionOrder(completionOrder++);
                if (firstFinishTeam == 0)
                {
                    firstFinishTeam = athlete.getTeam();
                    firstFinishAthleteId = athlete.getId();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // printAthletesPositions(); //test

        if (!raceFinishedFlag && isRaceFinished())
        {
            std::cout << "-----------------------------------------" << "\n";
            std::cout << "The first athlete finished the race." << "\n";
            std::cout << "Team " << firstFinishTeam << " - Athlete " << firstFinishAthleteId << " finished first and won 1st cup\n";
            printAthletesPositions();
            raceFinishedFlag = true;
        }

        for (const auto &request : athleteRequests)
        {
            printAthleteInfo(request.first, request.second);
        }
    }

    std::cout << "\nAll athletes have finished the race...\n\n";

    printTeamRankings();
}

bool Race::isRaceFinished() const
{
    for (const Athlete &athlete : athletes)
    {
        if (athlete.getPosition() >= 55000.0f)
        {
            return true;
        }
    }
    return false;
}

bool Race::allRaceFinished() const
{
    for (const Athlete &athlete : athletes)
    {
        if (!athlete.isCompleted())
        {
            return false;
        }
    }
    return true;
}

void Race::printAthletesPositions() const
{
    // std::vector<std::pair>
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Athlete Positions:\n";
    for (const Athlete &athlete : athletes)
    {
        std::cout << "Team " << athlete.getTeam() << " - Athlete " << athlete.getId() << ": Position " << athlete.getPosition() << "m, Speed: " << athlete.getSpeed();
        std::cout << '\n';
    }
    std::cout << "------------------------------------------------------";
    std::cout << "------------------------------------------------------\n";
}

void Race::printAthleteInfo(int teamId, int athleteId) const
{
    std::lock_guard<std::mutex> lock(mtx);
    for (const Athlete &athlete : athletes)
    {
        if (athlete.getTeam() == teamId && athlete.getId() == athleteId)
        {
            std::cout << "Team " << athlete.getTeam() << " - Athlete " << athlete.getId() << ": Position " << athlete.getPosition() << "m, Speed: " << athlete.getSpeed() << '\n';
            return;
        }
    }
    std::cout << "Invalid team or athlete id.\n";
}

void Race::printTeamRankings() const
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Team Rankings:\n";

    std::map<int, int> teamCompletionOrderSum;
    for (const Athlete &athlete : athletes)
    {
        if (athlete.isCompleted())
        {
            teamCompletionOrderSum[athlete.getTeam()] += athlete.getCompletionOrder();
        }
    }

    std::vector<std::pair<int, int>> sortedTeamCompletionOrderSum(teamCompletionOrderSum.begin(), teamCompletionOrderSum.end());
    std::sort(sortedTeamCompletionOrderSum.begin(), sortedTeamCompletionOrderSum.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              { return a.second < b.second; });

    for (const auto &team : sortedTeamCompletionOrderSum)
    {
        std::cout << "Team " << team.first << "\n";
    }

    auto &minpair = sortedTeamCompletionOrderSum.front();
    std::cout << "\n";
    std::cout << minpair.first << "th team to win and Won 2st cup";
}