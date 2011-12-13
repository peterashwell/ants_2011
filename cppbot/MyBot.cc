#include "GeneticBot.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    cout.sync_with_stdio(0); //this line makes your bot faster
    if(argc == 2)
    {	
      string filename = string(argv[1]);
      cerr << filename << endl;
      GeneticBot bot;
      bot.playGame(filename);
    }
    else
    {
      GeneticBot bot;
      bot.playGame("");
    }
    return 0;
}
