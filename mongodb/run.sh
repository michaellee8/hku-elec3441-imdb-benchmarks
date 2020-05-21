set -e
docker-compose --compatibility up -d db
sleep 20
docker-compose --compatibility up --build benchmark
docker-compose --compatibility down
