#!/bin/bash
rm -rf ./src
rm -rf ./server
rm -rf ./public
rm -rf ./config
cp -f ../../../../dashboard/package.json .
cp -f ../../../../dashboard/yarn.lock .
# cp -rf ../../../../dashboard/build .
cp -rf ../../../../dashboard/config .
cp -rf ../../../../dashboard/public .
cp -rf ../../../../dashboard/server .
cp -rf ../../../../dashboard/src .
