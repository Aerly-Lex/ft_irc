#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <password>"
    exit 1
fi

PASSWORD=$1

# Start User1
{
    echo "===> [User1] Connecting..." >&2
    echo -e "CAP LS\r\n"
    sleep 0.5
    echo -e "PASS $PASSWORD\r\n"
    sleep 0.5
    echo -e "NICK User1\r\n"
    sleep 0.5
    echo -e "USER User1 0 * :The First Test User\r\n"
    sleep 0.5
    echo -e "CAP END\r\n"
    sleep 0.5

    echo "===> [User1] Joining #testchannel..." >&2
    echo -e "JOIN #testchannel\r\n"
    sleep 0.5

    echo "===> [User1] Setting Operator (+o) on self..." >&2
    echo -e "MODE #testchannel +o User1\r\n"
    sleep 0.5

    echo "===> [User1] Setting Channel Topic..." >&2
    echo -e "TOPIC #testchannel :Dies ist ein Test Channel!\r\n"
    sleep 0.5

    sleep 5

    echo "===> [User1] Trying to Kick User2..." >&2
    echo -e "KICK #testchannel User2 :Raus hier!\r\n"
    sleep 2

    echo "===> [User1] Inviting User2 back..." >&2
    echo -e "INVITE User2 #testchannel\r\n"
    sleep 2

    echo "===> [User1] Sending PRIVMSG to #testchannel..." >&2
    echo -e "PRIVMSG #testchannel :Hallo nochmal!\r\n"
    sleep 2

    echo "===> [User1] Quitting..." >&2
    echo -e "QUIT :User1 sagt Goodbye\r\n"
} | nc localhost 6667 &

# Start User2
{
    sleep 2

    echo "===> [User2] Connecting..." >&2
    echo -e "CAP LS\r\n"
    sleep 0.5
    echo -e "PASS $PASSWORD\r\n"
    sleep 0.5
    echo -e "NICK User2\r\n"
    sleep 0.5
    echo -e "USER User2 0 * :The Second Test User\r\n"
    sleep 0.5
    echo -e "CAP END\r\n"
    sleep 0.5

    echo "===> [User2] Joining #testchannel..." >&2
    echo -e "JOIN #testchannel\r\n"
    sleep 5

    echo "===> [User2] (After Kick) Trying to Join #testchannel again (should fail unless invited)..." >&2
    echo -e "JOIN #testchannel\r\n"
    sleep 5

    echo "===> [User2] Joining after INVITE (should succeed)..." >&2
    echo -e "JOIN #testchannel\r\n"
    sleep 5

    echo "===> [User2] Sending PRIVMSG to #testchannel..." >&2
    echo -e "PRIVMSG #testchannel :Ich bin wieder da!\r\n"
    sleep 2

    echo "===> [User2] Quitting..." >&2
    echo -e "QUIT :User2 sagt Goodbye\r\n"
} | nc localhost 6667
