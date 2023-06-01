close all
clear 'mppt_test_data' 'mpptiv_test_data' 'iv_test_data' 'mppt_test_data_upanddown'

addpath("..\lab_data_logging")
try
    extract_data()
catch
    % Do nothing %
end

run_UI()
    
function run_UI()

    load('test_data.mat', 'mppt_test_data', 'mpptiv_test_data', 'iv_test_data');
    
    % Create a UI figure
    fig = uifigure('Name', 'Select Plots');
    fig.Position = [50 50 500 740];
    
    % Create a button group for iv test data plots
    ivButtonGroup = uibuttongroup(fig, 'Position', [10 490 480 230], 'Title', 'IV Test Data (Sweep Iout with Fixed Vin, using Power Supply)');

    % Create a list for selecting iv test data sets
    ivList = uilistbox(ivButtonGroup, 'Position', [320 10 150 190], 'Items', [iv_test_data(:).label], 'Multiselect','on');

    % Create buttons for iv test data plots
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [10 170 100 30], 'String', 'PV, IV, and Ripple', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'PV, IV, and Ripple'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [10 130 100 30], 'String', 'Efficiency vs Vin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Efficiency vs Vin'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [10 90 100 30], 'String', 'Pout vs Iout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Pout vs Iout'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [10 50 100 30], 'String', 'Iin vs Vout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Iin vs Vout'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [10 10 100 30], 'String', 'Iin vs Vin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Iin vs Vin'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [120 170 100 30], 'String', 'Efficiency vs Iout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Efficiency vs Iout'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [120 130 100 30], 'String', 'Efficiency vs Iin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Efficiency vs Iin'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [120 90 100 30], 'String', 'Iout vs Ripple Voltage', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Iout vs Ripple Voltage'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [120 50 100 30], 'String', 'Iout vs Vout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Iout vs Vout'));
    uicontrol(ivButtonGroup, 'Style', 'pushbutton', 'Position', [120 50 100 30], 'String', 'Iin vs Temperature', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(ivList.Value,iv_test_data, 'Iin vs Temperature'));

    % Create a button group for MPPTIV test data plots
    mpptivButtonGroup = uibuttongroup(fig, 'Position', [10 250 480 230], 'Title', 'MPPTIV Test Data (Sweep Iout with Fixed DAC Voltage, using Solar Panel)');
    
    % Create a list for selecting MPPTIV test data sets
    mpptivList = uilistbox(mpptivButtonGroup, 'Position', [320 10 150 190], 'Items', [mpptiv_test_data(:).label], 'Multiselect','on');
    
    % Create buttons for MPPTIV test data plots
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [10 170 100 30], 'String', 'PV, IV, and Ripple', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'PV, IV, and Ripple'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [10 130 100 30], 'String', 'Efficiency vs Vin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Efficiency vs Vin'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [10 90 100 30], 'String', 'Pout vs Iout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Pout vs Iout'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [10 50 100 30], 'String', 'Iout vs Vout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Iout vs Vout'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [10 10 100 30], 'String', 'Iin vs Vin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Iin vs Vin'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [120 170 100 30], 'String', 'Efficiency vs Iout', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Efficiency vs Iout'));
    uicontrol(mpptivButtonGroup, 'Style', 'pushbutton', 'Position', [120 130 100 30], 'String', 'Efficiency vs Iin', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptivList.Value,mpptiv_test_data, 'Efficiency vs Iin'));

    % Create a button group for MPPT test data plots
    mpptButtonGroup = uibuttongroup(fig, 'Position', [10 10 480 230], 'Title', 'MPPT Test Data (Sweep DAC Voltage with Fixed Iout, using Solar Panel)');

    % Create a list for selecting MPPT test data sets
    mpptList = uilistbox(mpptButtonGroup, 'Position', [320 10 150 190], 'Items', [mppt_test_data(:).label], 'Multiselect','on');
     
    % Create buttons for MPPT test data plots
    uicontrol(mpptButtonGroup, 'Style', 'pushbutton', 'Position', [10 170 100 30], 'String', 'PV, IV, and Ripple', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptList.Value,mppt_test_data, 'PV, IV, and Ripple'));
    uicontrol(mpptButtonGroup, 'Style', 'pushbutton', 'Position', [10 130 100 30], 'String', 'Efficiency', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptList.Value,mppt_test_data, 'Efficiency'));
    uicontrol(mpptButtonGroup, 'Style', 'pushbutton', 'Position', [10 90 100 30], 'String', 'Vin vs Vdac', 'HandleVisibility', 'off', 'Callback', @(src,evt) createPlot(mpptList.Value,mppt_test_data, 'Vin vs Vdac'));


end

function createPlot(selection, data, plot_type)

    figure

    for i = 1:length(selection)

        for j = 1:length(data)
            if strcmp(data(j).label, selection(i))
                break;      % Exit the loop
            end
        end

        if strcmp(plot_type, 'PV, IV, and Ripple')
            subplot(4, 1, 1:3)
            plotPVIV(data(j))
            hold on
            title("Solar Panel IV and PV Curves")
            subplot(4, 1, 4)
            plotData(data(j).Vin, data(j).VinPP)
            hold on
            % Add selection(i) to the legend
            legend(selection)
            title("Solar Panel Voltage Ripple")
            ylabel("Input Voltage Ripple (VPP)")
            grid on
        elseif strcmp(plot_type, 'Efficiency vs Vin')
            % Plot Efficiency
            plotEfficiency(data(j), data(j).Vin)
            hold on
            legend(selection)
            title("Converter Efficiency Versus Input Voltage")
            xlabel("Input Voltage (V)")
        elseif strcmp(plot_type, 'Efficiency vs Iout')
            % Plot Efficiency
            plotEfficiency(data(j), data(j).Iout)
            hold on
            legend(selection)
            title("Converter Efficiency Versus Output Current")
            xlabel("Output Current (A)")
        elseif strcmp(plot_type, 'Efficiency vs Iin')
            % Plot Efficiency
            plotEfficiency(data(j), data(j).Iin)
            hold on
            legend(selection)
            title("Converter Efficiency Versus Input Current")
            xlabel("Input Current (A)")
        elseif strcmp(plot_type, 'Pout vs Iout')
            % Plot Pout versus Iout
            Pout = multiplyValues(data(j).Iout, data(j).Vout);
            plotData(data(j).Iout, Pout)
            hold on
            legend(selection)
            title("Output Power Versus Output Current")
            xlabel("Output Current (A)")
            ylabel("Output Power (W)")
            grid on
        elseif strcmp(plot_type, 'Vin vs Vdac')
            plotData(data(j).Vin, data(j).Vdac)
            hold on
            grid on
            legend(selection)
            title("Solar Panel Voltage Versus DAC Voltage")
            xlabel("Input Voltage (V)")
            ylabel("DAC Voltage (V)")
        elseif strcmp(plot_type, 'Iout vs Vout')
            plotData(data(j).Iout, data(j).Vout)
            hold on
            grid on
            legend(selection)
            title("Output Current Versus Output Voltage")
            xlabel("Output Current (A)")
            ylabel("Output Voltage (V)")
        elseif strcmp(plot_type, 'Iin vs Vin')
            plotData(data(j).Iin, data(j).Vin)
            hold on
            grid on
            legend(selection)
            title("Input Current Versus Input Voltage")
            xlabel("Input Current (A)")
            ylabel("Input Voltage (V)")
        elseif strcmp(plot_type, 'Iin vs Vout')
            plotData(data(j).Iin, data(j).Vout)
            hold on
            grid on
            legend(selection)
            title("Input Current Versus Output Voltage")
            xlabel("Input Current (A)")
            ylabel("Output Voltage (V)")
        elseif strcmp(plot_type, "Iout vs Ripple Voltage")
            plotData(data(j).Iout, data(j).VoutPP)
            hold on
            grid on
            legend(selection)
            title("Output Current Versus Output Voltage Ripple")
            xlabel("Output Current (A)")
            ylabel("Output Voltage Ripple (V)")
        elseif strcmp(plot_type, "Iin vs Temperature")
            plotData(data(j).Iin, data(j).Temperature)
            hold on
            grid on
            legend(selection)
            title("Input Current Versus Temperature")
            xlabel("Input Current (A)")
            ylabel("Temperature (C)")
        else
            disp("Invalid plot type")
        end
    end
end

function plotPVIV(mppt_test_data)
    Pin = multiplyValues(mppt_test_data.Iin,mppt_test_data.Vin);
    yyaxis left
    hold on
    plotData(mppt_test_data.Vin,mppt_test_data.Iin)
    grid on
    xlabel("Input Voltage (V)")
    ylabel("Input Current (A)")
    yyaxis right
    hold on
    plotData(mppt_test_data.Vin,Pin)
    ylabel("Input Power (W)")
end

function plotEfficiency(mppt_test_data,xdata)
    Pin = multiplyValues(mppt_test_data.Iin,mppt_test_data.Vin);
    Pout = multiplyValues(mppt_test_data.Iout,mppt_test_data.Vout);
    Efficiency = multiplyValues(100,divideValues(Pout,Pin));
    plotData(xdata,Efficiency)
    grid on
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

function extract_data()

    mppt_test_data(1) = loadData("mppt_test_data\ltc3119mppt(0.1A)1.txt","Iout=0.1A");
    mppt_test_data(2) = loadData("mppt_test_data\ltc3119mppt(0.15A)1.txt","Iout=0.15A");
    mppt_test_data(3) = loadData("mppt_test_data\ltc3119mppt(0.2A)1.txt","Iout=0.2A");
    mppt_test_data(4) = loadData("mppt_test_data\ltc3119mppt(0.3A)1.txt","Iout=0.3A");
    mppt_test_data(5) = loadData("mppt_test_data\ltc3119mppt(0.4A)1.txt","Iout=0.4A");
    mppt_test_data(6) = loadData("mppt_test_data\ltc3119mppt(0.5A)1.txt","Iout=0.5A");
    mppt_test_data(7) = loadData("mppt_test_data\ltc3119mppt(0.6A)1.txt","Iout=0.6A");

    mpptiv_test_data(1) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC0.5V)1.txt","Vdac=0.5V");
    mpptiv_test_data(2) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC0.6V)1.txt","Vdac=0.6V");
    mpptiv_test_data(3) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC0.7V)1.txt","Vdac=0.7V");
    mpptiv_test_data(4) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC0.8V)1.txt","Vdac=0.8V");
    mpptiv_test_data(5) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC0.9V)1.txt","Vdac=0.9V");
    mpptiv_test_data(6) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC1.0V)1.txt","Vdac=1.0V");
    mpptiv_test_data(7) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC1.1V)1.txt","Vdac=1.1V");
    mpptiv_test_data(8) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC1.2V)1.txt","Vdac=1.2V");
    mpptiv_test_data(9) = loadData("mpptiv_test_data\ltc3119mpptiv(DAC1.3V)1.txt","Vdac=1.3V");
    mpptiv_test_data(10) = loadData("mpptiv_test_data\ltc3119mpptiv(DACHiZ)1.txt","Vdac=HiZ");

    iv_test_data(1) = loadData("iv_test_data\ltc3119iv(vin=3.0V)1.txt","Vin=3.0V");
    iv_test_data(2) = loadData("iv_test_data\ltc3119iv(vin=4.0V)1.txt","Vin=4.0V");
    iv_test_data(3) = loadData("iv_test_data\ltc3119iv(vin=6.0V)1.txt","Vin=6.0V");
    iv_test_data(4) = loadData("iv_test_data\ltc3119iv(vin=9.0V)1.txt","Vin=9.0V");
    iv_test_data(5) = loadData("iv_test_data\ltc3119iv(vin=12.0V)1.txt","Vin=12.0V");
    
    mppt_test_data_upanddown(1) = loadData("mppt_test_data_upanddown\ltc3119mppt_upanddown(0.4A).txt","Iout=0.4A");

    % Save the variables in a .mat file
    save('test_data.mat', 'mppt_test_data', 'mpptiv_test_data', 'iv_test_data', 'mppt_test_data_upanddown');
end