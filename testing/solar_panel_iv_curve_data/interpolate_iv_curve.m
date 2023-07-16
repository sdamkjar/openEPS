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
cellConfiguration = '1s1P';  % Example cell configuration

% Call the function to get interpolated data
interpolatedData = readIVCurve(file, surfaceArea_cm2, cellConfiguration);

% Calculate the power for each data point
power = interpolatedData.Voltage_V .* interpolatedData.Current_mA;

% Find the maximum power point (MPP)
[~, idxMPP] = max(power);
voltageMPP = interpolatedData.Voltage_V(idxMPP);
powerMPP = power(idxMPP);

% Set the figure dimensions for IEEE single column
figureWidth = 4; % inches
figureHeight = figureWidth * 0.75; % adjust the height proportionally as desired

% Figure 1: PV Curve (Power vs. Voltage)
figure('Units', 'inches', 'Position', [0, 0, figureWidth, figureHeight]);
plot(interpolatedData.Voltage_V, power);
ylim([0 1300])
hold on;
line([voltageMPP voltageMPP], [0 powerMPP], 'Color', 'k', 'LineStyle', '--');
plot(voltageMPP, powerMPP, 'ro', 'MarkerSize', 5, 'MarkerFaceColor', 'r');
text(voltageMPP, powerMPP, 'Max Power Point  ', 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'right');
text(voltageMPP, 0, 'V_{MP}', 'VerticalAlignment', 'top', 'HorizontalAlignment', 'center');
hold off;
xlabel('Voltage (V)');
ylabel('Power (mW)');
grid on;

% Set the x-axis ticks to show every 1 V
xticks = 0:1:3;
set(gca, 'XTick', xticks);

hold on

% Calculate PV curves for different temperatures
temperature = [-20, 80]; % Temperature conditions in degrees Celsius
temperatureCoeff_current = 10e-6; % Temperature coefficient for current density in A/cm^2/K
temperatureCoeff_voltage = -6e-3; % Temperature coefficient for voltage in V/K

for i = 1:numel(temperature)
    % Adjust the current and voltage values based on temperature coefficients
    adjustedCurrent = interpolatedData.Current_mA + temperatureCoeff_current * (28-temperature(i)) * surfaceArea_cm2;
    adjustedVoltage = interpolatedData.Voltage_V - temperatureCoeff_voltage * (28-temperature(i));
    % Calculate the power for the adjusted PV curve
    adjustedPower = adjustedVoltage .* adjustedCurrent;
    % Plot the adjusted PV curve
    plot(adjustedVoltage, adjustedPower);
    hold on;
end
hold off;
legend('28°C','','', '-20°C', '80°C', 'Location', 'northwest');

% Set the x-axis ticks to show every 1 V
xticks = 0:1:3;
set(gca, 'XTick', xticks);

% Save the PV curves figure as an SVG image
pvCurveSvgFileName = 'solar_cell_pv_curves.svg';
saveas(gcf, pvCurveSvgFileName, 'svg');

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
