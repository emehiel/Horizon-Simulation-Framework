clc; clear all; close all;

pathname = uigetdir('','Select schedule directory');



% plot power data
a1_power_data = importdata(fullfile(pathname,'Asset1_PowerStateData.txt'));
a2_power_data = importdata(fullfile(pathname,'Asset2_PowerStateData.txt'));
figure;
h_dod = subplot(2,1,1); hold on;
stairs(a1_power_data.data(1,:),a1_power_data.data(2,:),'r');
stairs(a2_power_data.data(1,:),a2_power_data.data(2,:),'b');
ylabel('Battery DOD');
xlabel('Simulation Time (s)');
    
h_solar = subplot(2,1,2); hold on;
stairs(a1_power_data.data(3,:),a1_power_data.data(4,:),'r');
stairs(a2_power_data.data(3,:),a2_power_data.data(4,:),'b');
ylim(1.2*ylim);
ylabel('Solar Panel Power In (W)');
xlabel('Simulation Time (s)');

% plot buffer and comm data
a1_ssdr_data = importdata(fullfile(pathname,'Asset1_SSDRStateData.txt'));
a1_comm_data = importdata(fullfile(pathname,'Asset1_CommStateData.txt'));
a2_ssdr_data = importdata(fullfile(pathname,'Asset2_SSDRStateData.txt'));
a2_comm_data = importdata(fullfile(pathname,'Asset2_CommStateData.txt'));
figure;
h_ssdr = subplot(2,1,1); hold on;
stairs(a1_ssdr_data.data(1,:),a1_ssdr_data.data(2,:),'r');
stairs(a2_ssdr_data.data(1,:),a2_ssdr_data.data(2,:),'b');
ylabel('Data Buffer Usage');
xlabel('Simulation Time (s)');
    
h_comm = subplot(2,1,2); hold on;
stairs(a1_comm_data.data(1,:),a1_comm_data.data(2,:),'r');
stairs(a2_comm_data.data(1,:),a2_comm_data.data(2,:),'b');
ylabel('Comm Rate (Mb/s)');
xlabel('Simulation Time (s)');


% plot eosensor data
% eos_data = importdata(fullfile(pathname,'Asset1_EOSensorStateData.txt'));
% figure;
% h_inc = subplot(2,1,1);
% bar(eos_data.data(1,:),eos_data.data(2,:));
% ylabel('Target Incidence Angle (deg)');
% xlabel('Simulation Time (s)');
%     
% h_pix = subplot(2,1,2);
% bar(eos_data.data(1,:),eos_data.data(2,:));
% ylabel('Pixels (#)');
% xlabel('Simulation Time (s)');