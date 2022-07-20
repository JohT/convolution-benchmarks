# Commands
This document lists the steps and commands that were executed to set up chart generation.

## Run benchmarks and generate charts

The following two commands are all that is needed to run the benchmark tests and generate the charts (svg and png).
All intermediate files and results are written to `..build/benchmark/`.
The final results will be copied into this folder.

```shell
npm install
npm run create-benchmark-charts
```

## Scripts
Here is a summary of all scripts listed in the [package.json](./package.json) file:

- **create-benchmark-charts**: Executes all scripts that are needed to run the benchmark tests and generate the reports and charts of them.
- **create-benchmark-directory**: Creates the directory `benchmark` inside the `build` directory using [mkdirp](https://www.npmjs.com/package/mkdirp).
- **run-benchmarks**: Runs the executable in `../build/test` with the [catch2](https://github.com/catchorg/Catch2/tree/v2.13.2) benchmark tests.
- **convert-xml-to-json**: Converts the benchmark results XML to JSON using [convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json).
- **generate-png-chart**: Generates a bar chart from the benchmark results and stores it as a PNG picture file using [vega-lite](https://vega.github.io/vega-lite).
- **generate-svg-chart**: Generates a bar chart from the benchmark results and stores it as a SVG vector graphics file using [vega-lite](https://vega.github.io/vega-lite).

## Setup commands
Here are some more commands that were used to setup chart generation for this project.

### Install vega-cli on mac os

The following commands were used to install [vega command line utilities](https://vega.github.io/vega/usage/#cli) in the root repository path.
Since it depends on [node-canvas](https://github.com/Automattic/node-canvas) the [node-canvas installation](https://github.com/Automattic/node-canvas#installation) might be needed in advance.

```shell
brew install pkg-config cairo pango libpng jpeg giflib librsvg pixman
npm install --save-dev canvas vega-cli
```

### Install vega-lite

In root repository path execute:

```shell
npm install --save-dev vega-lite
```

### Run benchmark tests with XML report output

```shell
./build/test/GettingStartedWithSIMDTests [performance] --reporter XML::out=./build/test/test-xml-report.xml --reporter console::out=-::colour-mode=ansi
```

The resulting XML contains all test results including benchmark values as the following snippet shows:
```xml
    <BenchmarkResults name="(JohT) kernelCentricConvolution" samples="100" resamples="100000" iterations="1" clockResolution="17.1192" estimatedDuration="6.7594e+06">
      <!-- All values in nano seconds -->
      <mean value="67934.4" lowerBound="67736.9" upperBound="68257.3" ci="0.95"/>
      <standardDeviation value="1267.56" lowerBound="870.005" upperBound="1755.71" ci="0.95"/>
      <outliers variance="0.113537" lowMild="0" lowSevere="1" highMild="2" highSevere="15"/>
    </BenchmarkResults>
```

### Convert XML to JSON

The following command uses [NPM convert-xml-to-json](https://www.npmjs.com/package/convert-xml-to-json) to convert a XML to a JSON file:
```shell
npx --yes convert-xml-to-json ../build/test/test-xml-report.xml test-xml-report.json
```

## Generate PNG image chart
The following command generates the PNG chart as described in [Compiling Vega-Lite to Vega using npx](https://vega.github.io/vega-lite/usage/compile.html#using-npx):

```shell
npx -p vega -p vega-lite vl2png --loglevel debug BenchmarkChart.json BenchmarkChart.png
```

## Generate SVG image chart
The following command generates the PNG chart as described in [Compiling Vega-Lite to Vega using npx](https://vega.github.io/vega-lite/usage/compile.html#using-npx):

```shell
npx -p vega -p vega-lite vl2svg --loglevel info BenchmarkChart.json BenchmarkChart.svg
```