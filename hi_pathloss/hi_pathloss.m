%% Reading data from BVH dataset

clear all
close all

tic

% choose the BVH input file; some samples from "mocap.cs.cmu.edu":
% 13_01: sit on high stool, stand up
% 13_04: sit on stepstool, chin in hand
% 35_01: walking

[file,path] = uigetfile('./*.*');
[filepath,name,ext] = fileparts(fullfile(path,file));
[skeleton,time] = loadbvh(fullfile(path,file));

local_avg_win = 10;

plot_on = false;
write_video = false;

if write_video
    frame_step = 10;
else
    frame_step = 1;
end

pathloss_res = zeros(size(time,2),3);
pathloss_res(:,1) = time;

%% Initialize skeleton figure

height = 175.7;
r_torso = 16.2;
r_head = 7;
r_shldr = 2;
r_arm = 5.5;
r_forearm = 4;
r_leg = 6;
r_shin = 5;

hip = 1;
chest = 3;
neck = 4;
head = 5;
leftEye = 6;
rightEye = 8;
rCollar = 10;
rShldr = 11;
rForeArm = 12;
rHand = 13;
lShldr = 30;
lForeArm = 31;
lHand = 32;
rThigh = 49;
rShin = 50;
rFoot = 51;
lButtock = 53;
lThigh = 54;
lShin = 55;
lFoot = 56;

height_raw = norm(skeleton(leftEye).Dxyz([1 2 3],1) - skeleton(lFoot).Dxyz([1 2 3],1));
wb = waitbar(1/skeleton(1).Nframes,'Making path loss model');

if plot_on
    h = figure(1); clf; hold on
    set(h,'color','white')
    view(-30,30)
    axis equal on
end

Njoints = numel(skeleton);

%% Process frame by frame

if write_video, vidObj = VideoWriter(name); open(vidObj); end 

for ff = 1:frame_step:skeleton(1).Nframes

  if plot_on    
    title(sprintf('%1.2f seconds',time(ff)))
  end
  
  for nn = 1:Njoints
    % scale up all joints to match the person's height
    skeleton(nn).Dxyz(:,ff) = skeleton(nn).Dxyz(:,ff) * height / height_raw;
    if plot_on
%         set(gca,'visible','off')
        hp(nn) = plot3(skeleton(nn).Dxyz(1,ff),skeleton(nn).Dxyz(2,ff),skeleton(nn).Dxyz(3,ff),'.','markersize',20);
    
        parent = skeleton(nn).parent;
        if parent > 0
          hl(nn) = plot3([skeleton(parent).Dxyz(1,ff) skeleton(nn).Dxyz(1,ff)],...
                         [skeleton(parent).Dxyz(2,ff) skeleton(nn).Dxyz(2,ff)],...
                         [skeleton(parent).Dxyz(3,ff) skeleton(nn).Dxyz(3,ff)]);
        end
    end
  end

  % draw body parts
  cyl_torso = [skeleton(hip).Dxyz([1 2 3],ff); skeleton(neck).Dxyz([1 2 3],ff); r_torso]';
  [s, trans] = drawCylinder(cyl_torso,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
  
  cyl_head = [(skeleton(leftEye).Dxyz([1 2 3],ff) + skeleton(rightEye).Dxyz([1 2 3],ff)) / 2; skeleton(neck).Dxyz([1 2 3],ff); r_head]';
  cyl_rShldr = [skeleton(rShldr).Dxyz([1 2 3],ff); skeleton(rCollar).Dxyz([1 2 3],ff); r_shldr]';
  cyl_rArm = [skeleton(rShldr).Dxyz([1 2 3],ff); skeleton(rForeArm).Dxyz([1 2 3],ff); r_arm]';
  cyl_rForearm = [skeleton(rForeArm).Dxyz([1 2 3],ff); skeleton(rHand).Dxyz([1 2 3],ff); r_forearm]';
  cyl_lShldr = [skeleton(lShldr).Dxyz([1 2 3],ff); skeleton(chest).Dxyz([1 2 3],ff); r_shldr]';
  cyl_lArm = [skeleton(lShldr).Dxyz([1 2 3],ff); skeleton(lForeArm).Dxyz([1 2 3],ff); r_arm]';
  cyl_lForearm = [skeleton(lForeArm).Dxyz([1 2 3],ff); skeleton(lHand).Dxyz([1 2 3],ff); r_forearm]';
  cyl_rThigh = [skeleton(rThigh).Dxyz([1 2 3],ff); skeleton(rShin).Dxyz([1 2 3],ff); r_leg]';
  cyl_rShin = [skeleton(rShin).Dxyz([1 2 3],ff); skeleton(rFoot).Dxyz([1 2 3],ff); r_leg]';
  cyl_lThigh = [skeleton(lThigh).Dxyz([1 2 3],ff); skeleton(lShin).Dxyz([1 2 3],ff); r_leg]';
  cyl_lShin = [skeleton(lShin).Dxyz([1 2 3],ff); skeleton(lFoot).Dxyz([1 2 3],ff); r_leg]';
  
  if plot_on
      drawCylinder(cyl_head,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_rShldr,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_rArm,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_rForearm,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_lShldr,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_lArm,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_lForearm,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_rThigh,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_rShin,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_lThigh,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
      drawCylinder(cyl_lShin,'FaceColor',[ 0.5843 0.8157 0.9882],'FaceAlpha',0.5);
  end
  
  % link between left hand and right thigh
  p_lhand = skeleton(lHand).Dxyz([1 2 3],ff)';
  p_rThigh = skeleton(rThigh).Dxyz([1 2 3],ff)';
  
  handThigh = createLine3d(p_lhand, p_rThigh);
  
  if plot_on
    drawLine3d(handThigh);
  end
  
  intersect_points = intersectLineCylinder(handThigh,cyl_torso);
  
  if size(intersect_points,1) > 1
    p1t = inv(trans) * [intersect_points(1,:)';1];
    p2t = inv(trans) * [intersect_points(2,:)';1];
    ang = atan2(norm(cross(p1t(1:3),p2t(1:3))),dot(p1t(1:3),p2t(1:3)));
    d_curv = sqrt(r_torso^2 * ang^2 + (p2t(3)-p1t(3))^2);
    d_free1 = distancePoints3d(p_lhand,intersect_points(1,:));
    d_free2 = distancePoints3d(p_rThigh,intersect_points(2,:));

    % curvature path loss around torso
    pl_curve = pathloss(4,1,d_curv*10,1,1);
    % free space path loss
    pl_fs = 20 * log10((d_free1 + d_free2)/100) + 20 * log10(2.4e9) - 147.55;
    % total path loss
    pl_tot = pl_curve + pl_fs;
    
  else
    % no intersection with torso
    d_free = distancePoints3d(p_lhand,p_rThigh); 
    pl_tot = 20 * log10(d_free/100) + 20 * log10(2.4e9) - 147.55;
  
  end
  
  pathloss_res(ff,2) = pl_tot;
  pathloss_res(ff,3) = skeleton(lHand).Dxyz(2,ff);
    
  if plot_on && ~write_video
%       view([25 75])
%       set(gca,'visible','off')
      waitforbuttonpress;
  end
  if write_video
      writeVideo(vidObj,getframe(gcf)); 
  end
  cla
  waitbar(ff/skeleton(1).Nframes,wb);
end

%% Generate plots and output path loss file

figure;
plot(pathloss_res(:,1), pathloss_res(:,2));
title('Pathloss');
xlabel('time(s)');
ylabel('Pathloss(dB)');
set(gca,'FontSize',20)

if write_video, close(vidObj); end 
close(wb);

%  calculate moving average
local_avg = zeros(size(pathloss_res,1),1);
local_avg(1:local_avg_win) = pathloss_res(1:local_avg_win,2);

for s = local_avg_win+1:length(local_avg)
    local_avg(s) = mean(pathloss_res(s-local_avg_win:s,2));
end
pathloss_res(:,2) = local_avg;

save(strcat(name,'_pl'),'pathloss_res');

% write to txt file
pl_forfile = zeros(size(pathloss_res,1),4);
for i = 1:size(pl_forfile,1)
    pl_forfile(i,:) = [0 pathloss_res(i,2) pathloss_res(i,2) 0];
end
writematrix(pl_forfile,strcat(name,'_pl.txt'))

toc
