close all

addpath("lab_data_logging")

data(1) = loadData("mppt_test_data2\ltc3119mppt(0.3A)1.txt");
data(2) = loadData("mppt_test_data2\ltc3119mppt(0.4A)1.txt");
data(3) = loadData("mppt_test_data2\ltc3119mppt(0.5A)1.txt");

for i = 1:3
    figure(1)
    hold on
    subplot(4,1,1:3)
    plotPVIV(data(i))
    
    subplot(4,1,4)
    plotData(data(i).Vin,data(i).VinPP)
    legend("Iout = 0.3 A","Iout = 0.4 A","Iout = 0.5 A")
    ylabel("Ripple Voltage (VPP)")
    ylim([0 0.5])
    grid on
    
    figure(2)
    hold on
    
    plotEfficiency(data(i))
    ylim([89 96])
    legend("Iout = 0.3 A","Iout = 0.4 A","Iout = 0.5 A")
end

function plotPVIV(data)
    Pin = multiplyValues(data.Iin,data.Vin);
    yyaxis left
    plotData(data.Vin,data.Iin)
    grid on
    xlabel("Voltage (V)")
    ylabel("Current (A)")
    yyaxis right
    plotData(data.Vin,Pin)
    ylabel("Power (W)")
end

function plotEfficiency(data)
    Pin = multiplyValues(data.Iin,data.Vin);
    Pout = multiplyValues(data.Iout,data.Vout);
    Efficiency = multiplyValues(100,divideValues(Pout,Pin));
    plotData(data.Vin,Efficiency)
    grid on
    xlabel("Voltage (V)")
    ylabel("Efficiency (%)")
end

function result = multiplyValues(value1, value2)
    % Check if the input vectors have the same length or if one of them is a scalar
    if ~(isnumeric(value1) || isnumeric(value2)) && numel(value1.value) ~= numel(value2.value)
        error('Input vectors must have the same length.');
    end

    % Calculate the resulting value
    if isnumeric(value1)
        result.value = value1 .* value2.value;
    elseif isnumeric(value2)
        result.value = value1.value .* value2;
    else
        result.value = value1.value .* value2.value;
    end

    % Calculate the standard deviation if both values have it
    if isfield(value1, 'std') && isfield(value2, 'std')
        result.std = zeros(size(result.value));
        for i = 1:numel(result.value)
            % Calculate the standard deviation using the formula for multiplication
            result.std(i) = abs(result.value(i)) .* sqrt((value1.std(i) / value1.value(i))^2 + (value2.std(i) / value2.value(i))^2);
        end
    end
end

function result = divideValues(value1, value2)
    % Check if the input vectors have the same length or if one of them is a scalar
    if ~(isnumeric(value1) || isnumeric(value2)) && numel(value1.value) ~= numel(value2.value)
        error('Input vectors must have the same length.');
    end

    % Calculate the resulting value
    if isnumeric(value1)
        result.value = value1 ./ value2.value;
    elseif isnumeric(value2)
        result.value = value1.value ./ value2;
    else
        result.value = value1.value ./ value2.value;
    end

    % Calculate the standard deviation if both values have it
    if isfield(value1, 'std') && isfield(value2, 'std')
        result.std = zeros(size(result.value));
        for i = 1:numel(result.value)
            % Calculate the standard deviation using the formula for division
            result.std(i) = abs(result.value(i)) .* sqrt((value1.std(i) / abs(value1.value(i)))^2 + (value2.std(i) / abs(value2.value(i)))^2);
        end
    end
end