#!/bin/bash

script_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$script_path"

PG_HOST=$1
PG_PORT=$2
PG_USER=$3
PG_PASS=$4

psql -h $1 "sslmode=disable user=$3 port=$2 password=$4" < 10-create-user-and-db.sql
for file in VersionScripts/v*; do
    psql -h $1 "sslmode=disable user=$3 port=$2 password=$4" < $file
done
