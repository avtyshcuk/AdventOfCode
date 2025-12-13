#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Reindeer {
    std::string name;
    int velocity;
    int maxVelocityTime;
    int restTime;
};

enum class State {
    FLYING,
    REST
};

struct ReindeerState {
    State state;
    int secondsUntilStateChange;
    int currentDistance;
    int velocity;
    int secondsToRest;
    int secondsToFly;
    int points;
};

std::vector<Reindeer> parse(const std::string& fileName) {
    std::ifstream input{fileName};
    if (!input) {
        throw std::runtime_error("File '" + fileName + "' is missing or invalid");
    }
    
    std::vector<Reindeer> reindeers;
    std::string line;
    
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        Reindeer reindeer;
        std::string can, fly, kms, _for, seconds1, but, then, must, rest, _for2, seconds2;
        
        iss >> reindeer.name >> can >> fly >> reindeer.velocity >> kms >> _for 
            >> reindeer.maxVelocityTime >> seconds1 >> but >> then >> must >> rest 
            >> _for2 >> reindeer.restTime >> seconds2;
        
        reindeers.push_back(reindeer);
    }
    
    return reindeers;
}

void printReindeers(const std::vector<Reindeer>& reindeers) {
    for (const auto& r : reindeers) {
        std::cout << r.name << " can fly " << r.velocity << " km/s for " 
                  << r.maxVelocityTime << " seconds, but then must rest for " 
                  << r.restTime << " seconds.\n";
    }
}

int part1(const std::vector<Reindeer>& reindeers) {
    const int RACE_DURATION = 1000;
    
    std::unordered_map<std::string, ReindeerState> states;
    for (const auto& reindeer : reindeers) {
        ReindeerState state;
        state.state = State::FLYING;
        state.secondsUntilStateChange = reindeer.maxVelocityTime;
        state.currentDistance = 0;
        state.velocity = reindeer.velocity;
        state.secondsToRest = reindeer.restTime;
        state.secondsToFly = reindeer.maxVelocityTime;
        state.points = 0;
        
        states[reindeer.name] = state;
    }
    
    for (int second = 0; second < RACE_DURATION; ++second) {
        for (auto& [name, state] : states) {
            if (state.state == State::FLYING) {
                state.currentDistance += state.velocity;
            }
            
            state.secondsUntilStateChange--;
            
            if (state.secondsUntilStateChange == 0) {
                if (state.state == State::FLYING) {
                    state.state = State::REST;
                    state.secondsUntilStateChange = state.secondsToRest;
                } else {
                    state.state = State::FLYING;
                    state.secondsUntilStateChange = state.secondsToFly;
                }
            }
        }
    }
    
    int maxDistance = 0;
    for (const auto& [name, state] : states) {
        maxDistance = std::max(maxDistance, state.currentDistance);
    }
    
    return maxDistance;
}

int part2(const std::vector<Reindeer>& reindeers) {
    const int RACE_DURATION = 2503;
    
    std::unordered_map<std::string, ReindeerState> states;
    for (const auto& reindeer : reindeers) {
        ReindeerState state;
        state.state = State::FLYING;
        state.secondsUntilStateChange = reindeer.maxVelocityTime;
        state.currentDistance = 0;
        state.velocity = reindeer.velocity;
        state.secondsToRest = reindeer.restTime;
        state.secondsToFly = reindeer.maxVelocityTime;
        state.points = 0;
        
        states[reindeer.name] = state;
    }
    
    for (int second = 0; second < RACE_DURATION; ++second) {
        for (auto& [name, state] : states) {
            if (state.state == State::FLYING) {
                state.currentDistance += state.velocity;
            }
            
            state.secondsUntilStateChange--;
            
            if (state.secondsUntilStateChange == 0) {
                if (state.state == State::FLYING) {
                    state.state = State::REST;
                    state.secondsUntilStateChange = state.secondsToRest;
                } else {
                    state.state = State::FLYING;
                    state.secondsUntilStateChange = state.secondsToFly;
                }
            }
        }
        
        int maxDistance = 0;
        for (const auto& [name, state] : states) {
            maxDistance = std::max(maxDistance, state.currentDistance);
        }
        
        for (auto& [name, state] : states) {
            if (state.currentDistance == maxDistance) {
                state.points++;
            }
        }
    }
    
    int maxPoints = 0;
    for (const auto& [name, state] : states) {
        maxPoints = std::max(maxPoints, state.points);
    }
    
    return maxPoints;
}

int main() {
    try {
        auto reindeers = parse("input14");
        
        std::cout << "Parsed " << reindeers.size() << " reindeers:\n";
        printReindeers(reindeers);
        std::cout << "\n";
        
        auto answer1 = part1(reindeers);
        std::cout << "Part 1: " << answer1 << "\n";
        
        auto answer2 = part2(reindeers);
        std::cout << "Part 2: " << answer2 << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
