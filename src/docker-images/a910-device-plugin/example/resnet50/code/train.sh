#!/bin/sh

if  [ x"${DEVICE_ID}" = x ] ;
then
    echo "turing train fail" >> /d_solution/train_${DEVICE_ID}.log
    exit
fi

export DEVICE_INDEX=$1
env > /d_solution/env_${DEVICE_ID}.log

#start fmk
rm -rf /code/${DEVICE_ID}/*
mkdir -p /code/${DEVICE_ID}
cd /code/${DEVICE_ID};

if [ x"${DEVICE_ID}" = x0 ] ;
then
    python3.6 /code/resnet50_train/mains/res50.py --config_file=res50_32bs_1p --max_train_steps=100000 --iterations_per_loop=100000 --debug=True --eval=False --model_dir=/d_solution/ckpt${DEVICE_ID} > /d_solution/train_${DEVICE_ID}.log 2>&1
    if [ $? -eq 0 ] ;
    then
        echo "turing train success" >> /d_solution/train_${DEVICE_ID}.log
    else
        echo "turing train fail" >> /d_solution/train_${DEVICE_ID}.log
    fi
else
    python3.6 /code/resnet50_train/mains/res50.py --config_file=res50_32bs_1p --max_train_steps=100000 --iterations_per_loop=100000 --debug=True --eval=False --model_dir=/d_solution/ckpt${DEVICE_ID} > /d_solution/train_${DEVICE_ID}.log 2>&1
    if [ $? -eq 0 ] ;
    then
        echo "turing train success" >> /d_solution/train_${DEVICE_ID}.log
    else
        echo "turing train fail" >> /d_solution/train_${DEVICE_ID}.log
    fi
fi
