#!/bin/bash

echo "STABLE_BUILD_GIT_DESCRIBE $(git --no-pager describe --always --dirty)"
echo "STABLE_BUILD_GIT_REV $(git rev-parse HEAD)"
