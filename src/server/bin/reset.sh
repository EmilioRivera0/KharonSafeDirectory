#!/bin/bash
user=sonny
chown $user:$user ./test/
chmod 700 ./test/
chown $user:$user ./test/f1.txt ./test/f2.csv ./test/f3.json
chmod 700 ./test/f1.txt ./test/f2.csv ./test/f3.json
