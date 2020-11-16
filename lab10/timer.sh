#!/bin/bash
set -eu

host='localhost'
port='8000'
num_reqs=3
timeout=6

print_usage() {
	printf "Usage: $0 [-h host] [-p port] [-n number of reqs] [-t timeout (seconds)]"
}

while getopts 'h:p:n:t:' flag; do
  case "${flag}" in
    h) host="${OPTARG}" ;;
    p) port="${OPTARG}" ;;
    n) num_reqs="${OPTARG}" ;;
    t) timeout="${OPTARG}" ;;
    *) print_usage
       exit 1 ;;
  esac
done

echo "Making $num_reqs consecutive requests..."
elapsed=0
for ((i=1;i<=$num_reqs;i++)); do
	echo -ne "Request $i/$num_reqs...\r"
	start_time="$(date +%s%3N)"
	curl -sS -m "$timeout" -o/dev/null "http://$host:$port/"
	end_time="$(date +%s%3N)"
	elapsed=$(($elapsed + $end_time - $start_time))
done
echo "Time elapsed: ${elapsed}ms"
