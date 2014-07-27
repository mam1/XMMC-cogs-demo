/* start the A cog */
    printf("starting cogA\n");
    start_cogA(&parA.A);
    while(lockset(parA.A.message_lock));
    printf("%s\n",parA.A.cog_message);
    lockclr(parA.A.message_lock);
/* start the B cog */
    printf("starting cogB\n");
    startB(&parB.B);
    while(lockset(parB.B.message_lock));
    printf("%s\n",parB.B.cog_message);
    lockclr(parB.B.message_lock);
/* start the C cog */
    printf("starting cogC\n");
    startC(&parC.C);
    while(lockset(parC.C.message_lock));
    printf("%s\n",parC.C.cog_message);
    lockclr(parC.C.message_lock);