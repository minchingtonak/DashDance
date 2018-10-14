clear;clc;

% import audio
[y,fs] = audioread("bassboosted.wav");
y = y(:,1);

% calculate volume from amplitude
dt = 1/fs;
t = 0:dt:(length(y)*dt)-dt;
volume = 20*log10(y);

% cubes wave values to accentuate difference between low and high amplitudes
processedY = y.^3;

% filters wave using a moving average filter
windowWidth = 7;
kernel = ones(windowWidth,1) / windowWidth;
averagedY = filter(kernel, 1, processedY);

% downsamples music so we don't overload CAN bus
downsampledY = downsample(averagedY, 1024);

% outputs amplitude values into txt file needed for parsing from C++ files
csvwrite(unitedwedance.txt', downsampledY);
