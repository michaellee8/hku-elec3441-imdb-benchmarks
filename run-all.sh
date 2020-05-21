#!/bin/bash
git submodule init --recursive
for dir in */ ; do
    if [[ -d "$dir" ]] && [[ -e "$dir/run.sh"]]; then
        echo "running in $d"
        cd "$d"
        ./run.sh
        cd "../"
    fi
done