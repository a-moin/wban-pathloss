function path_loss_in_db = pathloss(frequency, N_ch, distance,env,standard)
% === Path loss model based on measurement results in a hospital room ===
% frequency: frequency bands ( 1: 400MHz, 2: 600MHz, 3: 900MHz, 4: 2450MHz, 5: UWB)
% N_ch: # of path loss realizations
% distance: distance between Tx and Rx in mm
% env: environment (1:Hospital room, 2: Anechoic chamber)
% standard: standard (1:CM3A, 2: CM3B)
% =====================================================
% =================== Channel parameter ===============
% a and b: parameters in linear fitting
% sigma_s: standard deviation of a Normal distribution

if standard==1
    
if frequency == 1
    if env==1
a = 3.00;
b = 34.6;
sigma_s = 4.63;
    else
      a = 22.6;
      b = -7.85;
      sigma_s = 5.60;  
    end;
elseif frequency == 2
    if env ==1
a = 16.7;
b = -0.45;
sigma_s = 5.99;
    else
       a = 17.2;
b = 1.61;
sigma_s = 6.96; 
    end;    
elseif frequency == 3
    if env==1
a = 15.5;
b = 5.38;
sigma_s = 5.35;
    else
        a = 28.8;
b = -23.5;
sigma_s = 11.7;
    end;    
elseif frequency == 4
    if env==1
a = 6.60;
b = 36.1;
sigma_s = 3.80;
    else
      a = 29.3;
      b = -16.8;
sigma_s = 6.89;  
    end;    
elseif frequency == 5
    if env==1
a = 19.2;
b = 3.38;
sigma_s = 4.40;
    else
       a = 34.1;
       b = -31.4;
       sigma_s = 4.85; 
    end;    

end
% =====================================================
% =============== Path loss realizations ==============
% -- Path loss in dB
path_loss_in_db = a*log10(distance)+b+sigma_s*randn(1,N_ch);
% =====================================================
elseif standard ==2
    if frequency ~=3
        display(' CMB3 is only available at 915 MHz');
        path_loss_in_db=NaN;
    else
        %pathloss 
        P0=-1.9;
        m0=2.1;
        P1=-59.4;
        sigma_p=-59.4;
        eta_P=randn(1,N_ch);
        PL=-10*log10(10.^(P0/10).*exp(-m0.*distance/10/4.343)+10.^(P1/10))+sigma_p.*eta_P;
        %small-scale fading
        K0=40.1;
        mk=0.61;
        sigma_k=2.4;
        eta_k=randn(1,N_ch);
        K_db=K0-mk.*PL+sigma_k.*eta_k;
        
        path_loss_in_db=PL+K_db;
    end;    
    
    
end;    