#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <password>"
    exit 1
fi

PASSWORD=$1

# Start User1
{
    echo "===> [User1] Connecting..."
    echo "CAP LS"
    sleep 0.5
    echo "PASS $PASSWORD"
    sleep 0.5
    echo "NICK User1"
    sleep 0.5
    echo "USER User1 0 * :The First Test User"
    sleep 0.5
    echo "CAP END"
    sleep 0.5

    echo "===> [User1] Joining #testchannel..."
    echo "JOIN #testchannel"
    sleep 0.5

    echo "===> [User1] Setting Operator (+o) on self..."
    echo "MODE #testchannel +o User1"
    sleep 0.5

    echo "===> [User1] Setting Channel Topic..."
    echo "TOPIC #testchannel :Dies ist ein Test Channel!"
    sleep 0.5

    sleep 5 # Warten auf User2

    echo "===> [User1] Trying to Kick User2..."
    echo "KICK #testchannel User2 :Raus hier!"
    sleep 2

    echo "===> [User1] Inviting User2 back..."
    echo "INVITE User2 #testchannel"
    sleep 2

    echo "===> [User1] Sending PRIVMSG to #testchannel..."
    echo "PRIVMSG #testchannel :Hallo nochmal!"
    sleep 2

    echo "===> [User1] Quitting..."
    echo "QUIT :User1 sagt Goodbye"
} | nc localhost 6667 &

# Start User2
{
    sleep 2

    echo "===> [User2] Connecting..."
    echo "CAP LS"
    sleep 0.5
    echo "PASS $PASSWORD"
    sleep 0.5
    echo "NICK User2"
    sleep 0.5
    echo "USER User2 0 * :The Second Test User"
    sleep 0.5
    echo "CAP END"
    sleep 0.5

    echo "===> [User2] Joining #testchannel..."
    echo "JOIN #testchannel"
    sleep 5

    echo "===> [User2] (After Kick) Trying to Join #testchannel again (should fail unless invited)..."
    echo "JOIN #testchannel"
    sleep 5

    echo "===> [User2] Joining after INVITE (should succeed)..."
    echo "JOIN #testchannel"
    sleep 5

    echo "===> [User2] Sending PRIVMSG to #testchannel..."
    echo "PRIVMSG #testchannel :Ich bin wieder da!"
    sleep 2

    echo "===> [User2] Quitting..."
    echo "QUIT :User2 sagt Goodbye"
} | nc localhost 6667
