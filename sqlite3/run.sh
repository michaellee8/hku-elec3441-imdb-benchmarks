set -e
docker-compose --compatibility up --build benchmark
docker-compose --compatibility down
