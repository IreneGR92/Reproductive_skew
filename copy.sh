#!/bin/bash

# Remote server details
REMOTE_USER="your_remote_username"
REMOTE_HOST="your_remote_host"
REMOTE_DIR="/home/odin/results"

# Local directory
LOCAL_DIR="/home/odin/Results/Reproductive_skew_results/server(EPYC)"

# Copy files from remote server to local directory
scp -r ${REMOTE_USER}@${REMOTE_HOST}:${REMOTE_DIR}/* ${LOCAL_DIR}/

# Check if the copy was successful
if [ $? -eq 0 ]; then
    # Delete files from remote server
    ssh ${REMOTE_USER}@${REMOTE_HOST} "rm -rf ${REMOTE_DIR}/*"
else
    echo "File copy failed. No files were deleted from the remote server."
fi