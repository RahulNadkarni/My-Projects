close all
clear all
clc

fc = 1000e3;       % 1000 kHz carrier    
fs = 10 * fc;      % Sampling frequency (10 times higher than the highest frequency) 
dT = 1 / fs;
m = [6 0 4 -6 2];  % Message
fm = fc / 10;      % Message frequency (1/10 of carrier frequency)
t = 0 : 1 / fs : length(m) / fm - 1 / fs; % Time vector (s)

%% Message signal m(t)
xm = [];  % Message signal initiation
for ii = 1:length(m)
    xm = [xm m(ii) * ones(1, length(t) / length(m))];
end

%% Carrier signal
xc = cos(2 * pi * fc * t);  % Carrier signal

%% DSB-SC Modulation
s = xm .* xc;  % Modulated signal

figure(1)

% Transmitted message signal
subplot(4,1,1)
plot(t, s);
title('Message signal s(t)');
xlabel('Time (s)');
ylabel('Amplitude');

% Frequency domain of transmitted signal
S = fftshift(fft(s, length(s)) * dT);
W = linspace(-pi, pi, length(S)); % Digital angular frequency
w = W / dT; % Angular frequency (rad)
f = w / (2 * pi);  % Frequency (Hz)

subplot(4,1,2)
plot(f, abs(S));
title('Magnitude of Transmitted Signal |S(f)|');
xlabel('Frequency (Hz)');
ylabel('Amplitude');

%% Demodulation 
lo = cos(2 * pi * fc * t + pi / 3); % Local oscillator signal

v = s .* lo; % Demodulated signal
V = fftshift(fft(v, length(v)) * dT);

% Low pass filter
Hlp = zeros(size(f));
for jj = 1:length(f)
    if abs(f(jj)) < 500e3
        Hlp(jj) = 2;
    end
end

% Frequency domain demodulation 
V0 = Hlp .* V;

% Signal in time domain after filtering
v0 = ifft(ifftshift(V0)) / dT;

% Plotting results

% Time domain demodulated and low-pass filtered signal
subplot(4,1,3)
plot(t, v0);
title('Demodulated and Low-Pass Filtered Output Signal v0(t)');
xlabel('Time (s)');
ylabel('Amplitude');

% Corresponding frequency domain spectrum |Vo(f)|
subplot(4,1,4)
plot(f, abs(V0));
title('Frequency Domain Spectrum |Vo(f)|');
xlabel('Frequency (Hz)');
ylabel('Amplitude');

sgtitle('DSB-SC Modulation and Demodulation Process');
