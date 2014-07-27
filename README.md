XMMC-cogs-demo
==============
This is the simplest example I could come up for the following application architecture:
*1  XMMC process that handles human interaction.  
*3  independent processes running on 3 separate cogs.
It is a skeleton of an application has a xmmc process which starts and stops processes running on separate cogs.  The xmmc routine is a simple command processor.  It supports the following commands:
	startA - load cogA code into a cog and start it
	startB - load cogB code into a cog and start it
	startC - load cogC code into a cog and start it
 stopA  - stop the cog running cogA code
 stopB  - stop the cog running cogB code
 stopC  - stop the cog running cogC code
statA  - send a message to the cog running the cogA code requesting status
statB  - send a message to the cog running the cogB code requesting status
statC  - send a message to the cog running the cogC code requesting status
exit   - terminate application		
The code loaded in to the cogs is identical except for the message returned when quired for status.

