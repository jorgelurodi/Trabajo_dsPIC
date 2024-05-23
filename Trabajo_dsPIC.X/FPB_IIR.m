function Hd = FPB_IIR
%FPB_IIR Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.10 and Signal Processing Toolbox 8.6.
% Generated on: 23-May-2024 22:48:49

% Butterworth Bandpass filter designed using FDESIGN.BANDPASS.

% All frequency values are in Hz.
Fs = 12529;  % Sampling Frequency

N   = 60;   % Order
Fc1 = 710;  % First Cutoff Frequency
Fc2 = 750;  % Second Cutoff Frequency

% Construct an FDESIGN object and call its BUTTER method.
h  = fdesign.bandpass('N,F3dB1,F3dB2', N, Fc1, Fc2, Fs);
Hd = design(h, 'butter');

% [EOF]
fvtool(Hd)