#!/bin/bash
#
# main()
#

SELF=$(cd $(dirname $0); pwd -P)/$(basename $0)

function lauchServer()
{
  nohup ./gameserver > ./nohupLog  2>&1 &
}

case "${1:-''}" in
  'start')
      ulimit -c unlimited
      lauchServer
	  echo "start ok!"
      ;;
  'restart')
      killall gameserver
      sleep 2
      ulimit -c unlimited
      lauchServer
      echo "restart ok!"
      ;;
  'stop')
      killall gameserver
      echo "stop ok!"
      ;;
  'make')
      make -j8
      ;;
  'clean')
      #clean
      make clean
      ;;
  'remake')
      make clean
      make -j8
      ;;
  *)
      echo "Usage: $SELF start|restart|stop|make|clean|remake"
      exit 1
      ;;    
esac
