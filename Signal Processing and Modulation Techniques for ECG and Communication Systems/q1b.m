close all;
clear all;
clc;
set(0,'DefaultAxesFontSize',10);  % set the font size of plot window 

% Load ECG signal
load('ecg_signal.mat');

% Define parameters
R = 10e3; % 10 kOhms
C = 133e-9; % 133 nF
m = 0.9; % Filter parameter

% Extract noisy ECG signal and time vector
noisy_ecg = ecg;
dT = t(2) - t(1);

% Define the duration for plotting
t_limit = 2.5; % seconds
f_limit = 250; % Hz

% Fourier Transform
X_f = fftshift(fft(noisy_ecg)); % Apply fftshift() here
n = length(noisy_ecg);
fs = 1/dT; % Sampling frequency
f = (-n/2:n/2-1)*(fs/n); % Frequency vector

% Define the twin-T notch filter transfer function H(omega)
H = @(omega) ((1 + m) * ((2 * 1i * omega * R * C) .^ 2 + 1)) ./ ...
             ((2 * 1i * omega * R * C) .^ 2 + 4 * (1 - m) * 1i * omega * R * C + 1);

% Apply the twin-T notch filter
omega = 2 * pi * f; % Angular frequency vector
H_omega = H(omega);
Z_f = X_f .* H_omega;

% Inverse FFT to get the filtered ECG signal in time domain
z_t = ifft(ifftshift(Z_f));

% Plot the results
figure;

% Plot noisy ECG signal in time domain
subplot(4,1,1);
plot(t, noisy_ecg);
xlim([0 t_limit]);
xlabel('time (s)');
ylabel('x(t)');
title('Original ECG Signal');
grid on;

% Plot Fourier Transform of the noisy ECG signal
subplot(4,1,2);
plot(f, abs(X_f));
xlim([-f_limit f_limit]);
xlabel('frequency (Hz)');
ylabel('|X(f)|');
title('Original ECG Signal in Frequency Domain');
grid on;

% Plot the Fourier Transform of the filtered ECG signal
subplot(4,1,3);
plot(f, abs(Z_f));
xlim([-f_limit f_limit]);
xlabel('frequency (Hz)');
ylabel('|Z(f)|');
title('Filtered ECG Signal in Frequency Domain');
grid on;

% Plot the filtered ECG signal in time domain
subplot(4,1,4);
plot(t, real(z_t));
xlim([0 t_limit]);
xlabel('time (s)');
ylabel('z(t)');
title('Filtered ECG Signal');
grid on;
