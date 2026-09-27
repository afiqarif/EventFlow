#!/bin/bash
set -e

# Swap the placeholders in the template and output to the working directory
envsubst < /app/config.template.json > /app/config.json

# Start your C++ server using the correct runtime path
exec /app/eventflow_gateway