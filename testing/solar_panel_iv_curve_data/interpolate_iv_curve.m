% File: interpolate_iv_curve.m
% Date: 28-May-2023
% Author: Stefan Damkjar
%
% Copyright 2023 Stefan Damkjar
% 
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
% 
%     http://www.apache.org/licenses/LICENSE-2.0
% 
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.


% Main script

% Input variables
file = 'Spectro_Lab_XTJ_iv_curve.csv';
surfaceArea_cm2 = 26.62;  % Example surface area of the solar cell in cm^2
cellConfiguration = '2s1P';  % Example cell configuration

% Call the function to get interpolated data
interpolatedData = readIVCurve(file, surfaceArea_cm2, cellConfiguration);

% Display the interpolated data in a plot
figure;
plot(interpolatedData.Voltage_V, interpolatedData.Current_mA,'o-');
xlabel('Voltage (V)');
ylabel('Current (mA)');
title('IV Curve');
grid on;

% Save the interpolated data as a CSV file
csvFileName = 'interpolatedData.csv';
writetable(interpolatedData, csvFileName);
fprintf('Interpolated data saved as %s\n', csvFileName);


% Function to read IV curve data and interpolate
function interpolatedData = readIVCurve(file, surfaceArea_cm2, varargin)
    % Inputs:
    %   file: File path of the IV curve data file
    %   surfaceArea_cm2: Surface area of the solar cell in cm^2
    %   varargin: Optional input for cell configuration (e.g., '4s1P')
    % Outputs:
    %   interpolatedData: Table containing interpolated IV curve data
    
    % Read the table from the file
    data = readtable(file);
    
    % Extract the current density and voltage columns
    currentDensity = data.CurrentDensity_mA_cm2_;
    voltage = data.Voltage_V_;
    
    % Set default values for seriesCells and parallelCells
    seriesCells = 1;
    parallelCells = 1;
    
    % Check if cellConfiguration is provided as an input
    if nargin > 2
        cellConfiguration = varargin{1};
        % Extract the number of cells in series and parallel from the input string
        seriesCells = str2double(cellConfiguration(1));
        parallelCells = str2double(cellConfiguration(3));
    end
    
    % Multiply current density values by surface area and cells in parallel
    current = currentDensity * surfaceArea_cm2 * parallelCells;
    
    % Multiply voltage values by the number of cells in series
    voltage = voltage * seriesCells;
    
    % Interpolate the data for every integer value of current, including extrapolation to current = 0
    interpolatedCurrent = (0:ceil(max(current)))';
    interpolatedVoltage = interp1(current, voltage, interpolatedCurrent, 'linear', 'extrap');
    
    % Create the interpolated data table
    interpolatedData = table(interpolatedCurrent, interpolatedVoltage, 'VariableNames', {'Current_mA', 'Voltage_V'});

end
