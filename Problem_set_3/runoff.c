#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES]; // grid of voters and their candidate preference

// Candidates have name, vote count, eliminated status
typedef struct // currrent status of each candidate
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES]; // array of candidates

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++) // create candidate array
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: "); // get number of voters
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++) // ask preferences for each voter
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++) // ask current voter preference for each candidate
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name)) // call 'vote' function for each preference
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true) // Voting over, calculate until winner
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // check if valid vote
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i; // if valid name - add to 'preference' array
            // printf("Valid candidate %s %s %i\n", name, candidates[i].name, preferences[voter][rank]);
            return true;
        }
    }
    // record preference into array 'preferences'

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Reset votes in candidates array before each round is calculated
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].votes = 0;
    }

    // candidates array - input this round of votes into preferences array
    for (int i = 0; i < voter_count; i++) // loop through each voter
    {
        for (int j = 0; j < candidate_count; j++) // loop through ranked candidates, find first one not eliminated
        {
            int preference = preferences[i][j];
            if (candidates[preference].eliminated == false) // if not eliminated, add to candidates array
            {
                candidates[preference].votes ++;
                // printf("Candidate %i, %s %i ++\n", preference, candidates[preference].name, candidates[preference].votes);
                break; // break out of ranked candidate loop, to next voter
            }
        }

    }
    return;
}

// Print the winner of the election, if a candidate gets majority
bool print_winner(void)
{

    int majority = voter_count / 2;
    if (voter_count % 2 > 0) // if voter count odd number
    {
        majority ++;
    }
    else
    {
        majority ++;
    }
    // printf("Voter count %i, majority %i\n", voter_count, majority);
    // check if votes == more than half the voter_count
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes >= majority) //if candidate has a majority
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int lowest_votes;
    for (int i = 0; i < candidate_count - 1; i++) // loop to find first candidate not eliminiated
    {
        if (candidates[i].eliminated == false) // found 1st non eliminated candidate
        {
            for (int j = i + 1; j < candidate_count; j++) // find second candidate not eliminated
            {
                if (candidates[j].eliminated == false) // found the next 2 non eliminated candidates
                {
                    if (candidates[i].votes < candidates[j].votes)
                    {
                        lowest_votes = candidates[i].votes;
                    }
                    else
                    {
                        lowest_votes = candidates[j].votes;
                    }
                }
                else
                {
                    continue;
                } // skip if second in pair candidate is eliminated
            }
        }
        else
        {
            continue;
        } // skip if first in pair candidate is eliminated
    }
    // printf("Found lowest candidate; %s\n", candidates[lowest_candidate].name)
    return lowest_votes;
}

// Return true if the election is tied between ALL candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        if (candidates[i].eliminated == false) // compared i candidate if not eliminated
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (candidates[j].eliminated == false) // compare i with j candidate, if j not eliminated
                {
                    if (candidates[i].votes == candidates[j].votes) // comparison with non eliminated candidates
                    {
                        // printf("Tie found\n");
                        continue;
                    }
                    else
                    {
                        return false; // if any 1 pair of candidates is not at a tie
                    }
                }

            }
        }
    }
    return true; // if no else is found, all candidates are tied
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}