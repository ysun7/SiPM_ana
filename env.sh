#!/bin/bash
THIS=$(dirname ${BASH_ARGV[0]})
export anasoft=$(cd $THIS; pwd)
#echo "The current working directory $sipmana"
export PATH=$PATH:$anasoft
export PYTHONPATH=$PYTHONPATH:$anasoft
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$anasoft
# add lecrunch to $PATH and $PYTHONPATH
#export PATH=$sipmana:$PATH
#export PYTHONPATH=$sipmana:$PYTHONPATH
#echo ${BASH_SOURCE[0]}