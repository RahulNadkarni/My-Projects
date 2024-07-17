% Constants
R = 10e3; % Resistance in ohms
C = 133e-9; % Capacitance in farads
m_values = [0.8, 0.9]; % Values of m
f = linspace(0, 200, 1000); % Frequency range from 0 to 200 Hz
omega = 2 * pi * f; % Angular frequency

% Pre-allocate arrays for magnitude and phase responses
H_mag = zeros(length(m_values), length(omega));
H_phase = zeros(length(m_values), length(omega));

% Loop over m values
for k = 1:length(m_values)
    m = m_values(k);
    
    % Transfer function H(omega)
    numerator = (1 + m) * ((2j * omega * R * C).^2 + 1);
    denominator = (2j * omega * R * C).^2 + 4 * (1 - m) * j * omega * R * C + 1;
    H = numerator ./ denominator;
    
    % Magnitude and phase
    H_mag(k, :) = abs(H);
    H_phase(k, :) = angle(H);
end

% Plot magnitude response
figure;
for k = 1:length(m_values)
    subplot(2, 1, 1);
    plot(f, 20 * log10(H_mag(k, :)));
    hold on;
end
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Magnitude Response');
legend(arrayfun(@(m) sprintf('m = %.1f', m), m_values, 'UniformOutput', false));
grid on;

% Plot phase response
for k = 1:length(m_values)
    subplot(2, 1, 2);
    plot(f, rad2deg(H_phase(k, :)));
    hold on;
end
xlabel('Frequency (Hz)');
ylabel('Phase (degrees)');
title('Phase Response');
legend(arrayfun(@(m) sprintf('m = %.1f', m), m_values, 'UniformOutput', false));
grid on;
