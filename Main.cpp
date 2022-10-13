/**
 * Daniel Vayman
 * dav210004
 */

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

struct Player
{
    string name = "";
    int hits = 0;
    int outs = 0;
    int strikeouts = 0;
    int walks = 0;
    int hits_by_pitch = 0;
    int sacrifices = 0;

    int plate_appearances = 0;
    int at_bats = 0;

    double BA = 0.0; //batting average
    double OB = 0.0; //on-base percentage
};

void sortRecord(Player&, string);
void displayLeaders(vector<Player>[]);
void displayStats(Player players[]);
void findLeaders(Player players[], vector<Player>[]);

int main()
{
    cout << "What is the file name? " << endl;
    string fileName = "";
    cin >> fileName;
    cout << endl;

    ifstream file;
    file.open(fileName); //opens file input stream
    
    /**
     * while the file has input
     * create a struct, pass in the name, then pass the struct by reference
     * to the sorting function to fill in all the other relevant data from the record
     */
    Player players[30]; //array used to hold all inputted players
    string playerName, record, line;
    int i = 0;
    while(getline(file, line)) //scans each line of the file until no lines are left to scan
    {
        playerName = line.substr(0, line.find(' ')); //grabs the player name
        record = line.substr(line.find(' ') + 1); //grabs the player record
        Player tempPlayer;
        tempPlayer.name = playerName;
        sortRecord(tempPlayer, record); //sorts the array of characters known as the player's record
        players[i] = tempPlayer; //adds the sorted player struct to the array of players
        i++;
    }
    file.close();

    vector<Player> leaders[6] = {};
    findLeaders(players, leaders); //finds the leaders for each value and stores it in a leader vector
    displayStats(players);
    displayLeaders(leaders);
}

/**
 * @brief sorts each record passed (string) and adds the data to the referenced player struct
 * 
 * @param player individual player struct
 * @param record string of characters that represents a players record
 */
void sortRecord(Player& player, string record)
{
    bool invalid = false;
    bool atbat = false;
    for(char x : record) //increase the count of each value whenever that character appears in the string
    {
        switch (x)
        {
        case 'H':
            player.hits++; //if the character is an H, incremenent the hit count
            atbat = true;
            break;
        case 'O':
            player.outs++; //if the character is an O, incremenent the out count
            atbat = true;
            break;
        case 'K':
            player.strikeouts++; //if the character is an K, incremenent the strikeout count
            atbat = true;
            break;
        case 'W':
            player.walks++; //if the character is an W, incremenent the walks count
            break;
        case 'P':
            player.hits_by_pitch++; //if the character is an P, incremenent the hits by pitch count
            break;
        case 'S':
            player.sacrifices++; //if the character is an S, incremenent the sacrifice count
            break;
        default:
            invalid = true; //if the character isn't recognized, then it must be invalid
            break;
        }
        if(!invalid) player.plate_appearances++; //if its a valid stat, then its a plate appearance
        if(atbat) player.at_bats++; //if its a hit, out, or strikeout, its an at bat
        invalid = false; //resets both values for the next character
        atbat = false;
    }
    if(player.at_bats > 0) player.BA = (double)player.hits/player.at_bats; //calculates BA (Batting Average) [Hits/At-Bats]
    if(player.plate_appearances > 0) player.OB = (player.hits + player.walks + player.hits_by_pitch) / (double)player.plate_appearances; //calculates OB [(hits + walks + hits by pitch) / plate-appearances]

    return;
}

/**
 * @brief given all the players, it sorts through 6 values (BA, OB, H, BB, K, HBP), finds the leaders, and adds the leading
 * players to their respective value vectors
 * 
 * @param players array including all players inputted from the file
 */
void findLeaders(Player players[], vector<Player> leaders[])
{
    //sets the default leader values to the first player in this list. This enables value comparison with other players.

    /**
     * @brief sets the default leader values to the first player in this list. This enables value comparison with other players.
     * 
     * BA: [0]
     * OB: [1]
     * H: [2]
     * BB: [3]
     * K: [4]
     * HBP: [5]
     */
    leaders[0].push_back(players[0]);
    leaders[1].push_back(players[0]);
    leaders[2].push_back(players[0]);
    leaders[3].push_back(players[0]);
    leaders[4].push_back(players[0]);
    leaders[5].push_back(players[0]);

    int i = 0;
    while(players[i].name != "")
    {
        // BA
        if(players[i].BA > leaders[0].front().BA) //if this player has a larger value, the leading player(s) get replaced
        {
            leaders[0].clear();
            leaders[0].push_back(players[i]);
        }
        else if (players[i].BA == leaders[0].front().BA && players[i].name != leaders[0].front().name) //if this player holds the same leading value, add them to the list of leaders
        {
            leaders[0].push_back(players[i]);
        }

        // OB
        if(players[i].OB > leaders[1].front().OB)
        {
            leaders[1].clear();
            leaders[1].push_back(players[i]);
        }
        else if (players[i].OB == leaders[1].front().OB && players[i].name != leaders[1].front().name)
        {
            leaders[1].push_back(players[i]);
        }
        
        // H
        if(players[i].hits > leaders[2].front().hits)
        {
            leaders[2].clear();
            leaders[2].push_back(players[i]);
        }
        else if (players[i].hits == leaders[2].front().hits && players[i].name != leaders[2].front().name)
        {
            leaders[2].push_back(players[i]);
        }

        // BB
        if(players[i].walks > leaders[3].front().walks)
        {
            leaders[3].clear();
            leaders[3].push_back(players[i]);
        }
        else if (players[i].walks == leaders[3].front().walks && players[i].name != leaders[3].front().name)
        {
            leaders[3].push_back(players[i]);
        }

        // K
        if(players[i].strikeouts < leaders[4].front().strikeouts)
        {
            leaders[4].clear();
            leaders[4].push_back(players[i]);
        }
        else if (players[i].strikeouts == leaders[4].front().strikeouts && players[i].name != leaders[4].front().name)
        {
            leaders[4].push_back(players[i]);
        }

        // HBP
        if(players[i].hits_by_pitch > leaders[5].front().hits_by_pitch)
        {
            leaders[5].clear();
            leaders[5].push_back(players[i]);
        }
        else if (players[i].hits_by_pitch == leaders[5].front().hits_by_pitch && players[i].name != leaders[5].front().name)
        {
            leaders[5].push_back(players[i]);
        }
        i++;
        if(i == 30) break; //ensures that an array index of 30 isnt accessed since it was never allocated
    }

    return;
}

/**
 * @brief displays the stats for each player
 * 
 * @param players collection of players
 */
void displayStats(Player players[])
{
    int i = 0;
    while(players[i].name != "")
    {
        cout << players[i].name << endl;
        cout << "BA: " << setprecision(3) << fixed << round(players[i].BA*1000)/1000.0 << endl;
        cout << "OB%: " << setprecision(3) << fixed << round(players[i].OB*1000)/1000.0 << endl;
        cout << "H: " << players[i].hits << endl;
        cout << "BB: " << players[i].walks << endl;
        cout << "K: " << players[i].strikeouts << endl;
        cout << "HBP: " << players[i].hits_by_pitch << endl << endl;
        i++;
        if(i == 30) break; ///ensures that an array index of 30 isnt accessed since it was never allocated
    }

    return;
}

/**
 * @brief displays the leaders of each value
 * 
 */
void displayLeaders(vector<Player> leaders[])
{
    cout << "LEAGUE LEADERS" << endl;

    cout << "BA: ";
    for(Player x : leaders[0]) //for each player in the leaders[0] vector, print name
    {
        cout << (x.name == leaders[0].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << setprecision(3) << fixed << round(leaders[0].front().BA*1000)/1000.0 << endl;

    cout << "OB%: ";
    for(Player x : leaders[1]) //for each player in the leaders[1] vector, print name
    {
        cout << (x.name == leaders[1].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << setprecision(3) << fixed << round(leaders[1].front().OB*1000)/1000.0 << endl;

    cout << "H: ";
    for(Player x : leaders[2]) //for each player in the leaders[2] vector, print name
    {
        cout << (x.name == leaders[2].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << leaders[2].front().hits << endl;

    cout << "BB: ";
    for(Player x : leaders[3]) //for each player in the leaders[3] vector, print name
    {
        cout << (x.name == leaders[3].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << leaders[3].front().walks << endl;

    cout << "K: ";
    for(Player x : leaders[4]) //for each player in the leaders[4] vector, print name
    {
        cout << (x.name == leaders[4].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << leaders[4].front().strikeouts << endl;

    cout << "HBP: ";
    for(Player x : leaders[5]) //for each player in the leaders[5] vector, print name
    {
        cout << (x.name == leaders[5].front().name ? "" : ", ") << x.name; //if the player isn't the first to be printed, add a comma before
    }
    cout << " " << leaders[5].front().hits_by_pitch << endl;

    return;
}

