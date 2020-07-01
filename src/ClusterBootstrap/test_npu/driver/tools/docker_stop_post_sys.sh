#!/bin/sh

prog_exit() {
    echo "=== begin proc exit    ==="
    echo "=== begin stop slogd   ==="
    ps -ef | grep slogd | grep -v grep | awk '{print $2}' | xargs kill -15
    echo "===   end pro exit     ==="
}
prog_exit
