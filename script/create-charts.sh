#!/bin/sh -x

# Run this script from the root directory of this repository

# Change into the chart directory
cd chart

# Installs all dependencies using Node.js Package Manager NPM
npm ci

# Creates the group folder for the current system and generates the SVG chart based on the benchmark results
npm run build