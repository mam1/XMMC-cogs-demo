XMMC-cogs-demo
==============
This is the simplest example I could come up for the following application architecture:

*	1 XMMC process that handles human interaction  
*	3 independent processes running on 3 separate cogs

This code is a skeleton application which has a xmmc process that is a simple command processor.  It can start, stop or query any of three cogs.  It supports the following commands:
* startA - load cogA code into a cog and start it
* startB - load cogB code into a cog and start it
* startC - load cogC code into a cog and start it
* stopA  - stop the cog running cogA code
* stopB  - stop the cog running cogB code
* stopC  - stop the cog running cogC code
* A  - send a message to the cog running the cogA code requesting status
* B  - send a message to the cog running the cogB code requesting status
* C  - send a message to the cog running the cogC code requesting status
* exit   - terminate application

The code loaded in to the cogs loops, checking its control block for a request for status.  If it get one, it writes a message to a buffer in its control block and resets the request flag. The code running in each cog is identical except for the message returned when quired.

