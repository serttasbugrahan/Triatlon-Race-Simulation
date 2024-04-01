#ifndef TRIATLON_H
#define TRIATLON_H

std::mutex mtx;
bool raceFinishedFlag = false;

class Athlete
{
public:
    Athlete(int _id, int _team, float _speed) : id(_id), team(_team), speed(_speed), position(0.0f), completionOrder(0), completed(false), speedUpdated{0} {}

    void move(float distance)
    {
        if (!completed)
        {
            position += distance;

            if (position < 5000.0f && speedUpdated == 0)
            {
                speedUpdated++;
            }
            if (position >= 5000.0f && position < 45000.0f && speedUpdated == 1)
            {
                speed *= 3.0;
                speedUpdated++;
            }
            if (position >= 45000.0f && speedUpdated == 2)
            {
                speed /= 9.0;
                speedUpdated++;
            }

            if (!completed && position >= 55000.0f)
            {
                completed = true;
                setCompletionOrder(completionOrder++);
            }
        }
    }

    float getPosition() const
    {
        return position;
    }

    float getSpeed() const
    {
        return speed;
    }

    int getId() const
    {
        return id;
    }

    int getTeam() const
    {
        return team;
    }

    bool isCompleted() const
    {
        return completed;
    }

    int getCompletionOrder() const
    {
        return completionOrder;
    }

    void setCompletionOrder(int order)
    {
        completionOrder = order;
    }

private:
    int id;
    int team;
    double speed;
    float position;
    int completionOrder;
    bool completed;
    int speedUpdated;

};

class Race
{
public:
    std::vector<Athlete> athletes;

    Race(int numTeams, int athletesPerTeam);

    void simulateRace();

    bool isRaceFinished() const;

    bool allRaceFinished() const;

    void printAthletesPositions() const;

    void printAthleteInfo(int teamId, int athleteId) const;

    void printTeamRankings() const;
};

#include "triatlon.cpp"

#endif
