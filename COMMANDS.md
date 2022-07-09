# Commands
This document lists the steps and commands that were executed to set up the project.
It also contains the commands that are used to build it.

## Tests with XML report

```shell
./build/test/GettingStartedWithSIMDTests --reporter XML::out=./build/test/test-xml-report.xml --reporter console::out=-::colour-mode=ansi
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