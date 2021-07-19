#!/bin/bash
curl -i -H "Content-Type: application/json" --data '{"plain":"neco"}' -X POST 0.0.0.0:80/hash
