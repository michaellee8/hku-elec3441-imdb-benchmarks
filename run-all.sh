#!/bin/bash
git submodule init
git submodule update
for dir in */ ; do
    if [[ -d "$dir" ]] && [[ -e "$dir/run.sh" ]]; then
        echo "running in $dir"
        cd "$dir"
        ./run.sh
        cd "../"
    fi
done
