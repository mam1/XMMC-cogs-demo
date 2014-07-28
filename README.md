XMMC-cogs-demo
==============
This an example of an application architecture that has 1 XMMC process that handles human interaction and 3 independent processes running on 3 separate cogs. The xmmc process is a simple command processor.  It can start, stop or dispaly status for any of three cogs.  It supports the following commands:
* startA - load cogA code into a cog and start it
* startB - load cogB code into a cog and start it
* startC - load cogC code into a cog and start it
* stopA  - stop the cog running cogA code
* stopB  - stop the cog running cogB code
* stopC  - stop the cog running cogC code
* queryA - set flag telling the cogA code to increment query counter for the cog
* queryB - set flag telling the cogB code to increment query counter for the cog
* queryC - set flag telling the cogC code to increment query counter for the cog
* status - display the status of all cogs and query counters
* exit   - terminate application

The code loaded in to the cogs zeros the cog query counter then loops, checking its control block for a request to increment the query counter for the cog.  If it get one, it resets the flag then increments the query counter in its control block. 


