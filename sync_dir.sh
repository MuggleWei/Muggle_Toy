#!/bin/bash
python "${PWD}/sync_dir.py" $1 $2

# can use function below directly in linux
# rsync -a --delete $1 $2