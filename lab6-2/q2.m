% Script to calculate and plot the energy of time-domain and frequency-domain signals
close all;
clear all;
clc;
set(0, 'DefaultAxesFontSize', 10);  % Set the font size of plot window 

% Load ECG signal
load('ecg_signal.mat'); % Make sure 'ecg_signal.mat' contains variables 'ecg' and 't'

% Define parameters
R = 10e3; % 10 kOhms
C = 133e-9; % 133 nF
m = 0.9; % Filter parameter

% Extract noisy ECG signal and time vector
noisy_ecg = ecg;
dT = t(2) - t(1); % Sampling period

% Define the duration for plotting
t_limit = 2.5; % seconds
f_limit = 250; % Hz

% Truncate the signals to the desired time range
time_idx = t <= t_limit;
noisy_ecg_truncated = noisy_ecg(time_idx);
t_truncated = t(time_idx);

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

% Truncate the filtered signal to the desired time range
z_t_truncated = z_t(time_idx);

% Calculate energy in time domain
energy_x_time = sum(abs(noisy_ecg_truncated).^2) * dT;
energy_z_time = sum(abs(z_t_truncated).^2) * dT;

% Calculate energy in frequency domain using Parseval's theorem
energy_x_freq = sum(abs(X_f).^2) * dT / n;
energy_z_freq = sum(abs(Z_f).^2) * dT / n;

% Calculate the energy spectrum
energy_spectrum_X = abs(X_f).^2 * (fs / n);
energy_spectrum_Z = abs(Z_f).^2 * (fs / n);

% Plot the energy spectrum
figure;
subplot(2,1,1);
plot(f, energy_spectrum_X);
xlim([-f_limit f_limit]);
xlabel('Frequency (Hz)');
ylabel('Energy(X(f))');
title('Energy Spectrum of Original ECG Signal');
grid on;

subplot(2,1,2);
plot(f, energy_spectrum_Z);
xlim([-f_limit f_limit]);
xlabel('Frequency (Hz)');
ylabel('Energy(Z(f))');
title('Energy Spectrum of Filtered ECG Signal');
grid on;

% Print energy calculations
fprintf('Energy of x(t) in time domain: %f\n', energy_x_time);
fprintf('Energy of X(f) in frequency domain: %f\n', energy_x_freq);
fprintf('Energy of z(t) in time domain: %f\n', energy_z_time);
fprintf('Energy of Z(f) in frequency domain: %f\n', energy_z_freq);
