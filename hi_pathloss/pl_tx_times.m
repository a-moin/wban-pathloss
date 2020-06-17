close all
clear all

local_avg_win = 10;
fourier_win = 100;
first_pkt = 3.033;
dt = 0.0083;

% [file,path] = uigetfile();
% load(fullfile(path,file))
% [filepath,name,ext] = fileparts(fullfile(path,file));
load('143_32_mdf_pl.mat')
time = pathloss_res(1:end-62,1);
% time = pathloss_res(1:599,1);
pathloss_res = pathloss_res(63:size(pathloss_res,1),:);
% pathloss_res = pathloss_res(63:663,:);
x = pathloss_res(:,3)/100;
v = (x(2:end)-x(1:end-1))/dt;
% accel = (v(2:end)-v(1:end-1))/dt;
accel = movmean((v(2:end)-v(1:end-1))/dt,50);
pl = -pathloss_res(:,2);
% pl = -pathloss_res(1:599,2);

% plot pos
figure;
plt1 = subplot(3,1,1);
plot(time(1:end),x);
grid on

% plot accel
plt2 = subplot(3,1,2);
plot(time(1:end-2),accel);
grid on

[pks,loc] = findpeaks(accel,time(1:end-2),'MinPeakDistance',0.5,'MinPeakProminence',5,'Annotate','peaks');
for i = 1:length(loc)
   line([loc(i) loc(i)], [pks(i)+2 pks(i)+5]) ;
end

plt3 = subplot(3,1,3);
plot(time,pl);
grid on
title('Pathloss');
xlabel('time(s)');
ylabel('Pathloss(dB)');
linkaxes([plt1,plt2,plt3],'x')
set([plt1,plt2,plt3],'FontSize',20)

% % find the period based on curve fitting and fourier main frequency
% f = fit(time(local_avg_win+1:fourier_win),local_avg(local_avg_win+1:fourier_win),'fourier2');
% pkt_period = 2*pi/f.w;
% tx_times = first_pkt:pkt_period:time(end);
% 
% for i = 1:length(tx_times)
%    line([tx_times(i) tx_times(i)], [-70 -85]) ;
% end

% % find the peaks in accel, and start with an initial low loss moment
% loc_diff = diff(loc);
% interval = zeros(length(loc)-2,1);
% for i = 1:length(loc)-2
%    interval(i) = mean(loc_diff(i:i+1));
% end
% 
% tx_points = zeros(length(loc)-1,1);
% tx_points(1) = first_pkt;
% for i=2:length(loc)-2
%     tx_points(i) = tx_points(i-1) + interval(i-1);
% end
% 
% for i = 1:length(tx_points)
%    line([tx_points(i) tx_points(i)], [-70 -85]) ;
% end

tx_points = zeros(length(loc)-2,1);
for i=1:length(loc)-2
%     tx_points(i) = loc(i+2) + ((loc(i+2) - loc(i+1))*0.5 + (loc(i+1) - loc(i))*0.5) * 0.8287;  % special for 143_32
%     tx_points(i) = loc(i+1) + (loc(i+1) - loc(i))*0.863;  % special for 143_32
    tx_points(i) = loc(i+1) + (loc(i+1) - loc(i))*0.344;  % special for 143_32
%     line([tx_points(i) tx_points(i)], [-70 -85]) ;
    [v,ind] = min(abs(time-tx_points(i)));
    line([tx_points(i) tx_points(i)], [pl(ind)+1 pl(ind)+3]) ;
end

% the output file for Castalia would have the first packet time in first
% line, and then diffs in the next lines

tx_points_forfile = [tx_points(1); diff(tx_points)];

writematrix(tx_points_forfile,'143_32_mdf_pl_tx.txt')

% % write matrix to txt file
% pl_forfile = zeros(size(pathloss_res,1),4);
% for i = 1:size(pl_forfile,1)
%     pl_forfile(i,:) = [0 pathloss_res(i,2) pathloss_res(i,2) 0];
% end
% writematrix(pl_forfile,'143_32_mdf_pl2.txt')