%Se trabaja con entrada2.csv y filtrada2.csv (filtrada con el pasoalto)

N = 256; %Número de muestras
fs = 10e3; %Frecuencia de muestreo
Ts = 1/fs; %Período de muestreo

tiempo = linspace(0,N*Ts,N); %Eje de tiempo
entrada = entrada'; %Al importar la columna con los datos de la señal de entrada, el vector lo llamo "entrada"
maximo = max(entrada);
entrada_normalizada = entrada./maximo;

% Reducir la cantidad de puntos a graficar para representar menos ciclos
% Por ejemplo, solo graficar los primeros 100 puntos
num_muestras_para_graficar = 100;
tiempo_reducido = tiempo(1:num_muestras_para_graficar);
entrada_reducida = entrada_normalizada(1:num_muestras_para_graficar);

%%% REPRESENTACIÓN TEMPORAL DE LA SEÑAL DE ENTRADA -> SUMA DE TRES TONOS
%%% (123 Hz, 367 Hz y 847 Hz) %%%%%%%
%plot(tiempo,entrada_normalizada,'LineWidth',2)
%title('Triangular entrada')
%xlabel('Tiempo [s]');
%ylabel('Ampitud [Norm]');
%grid on
plot(tiempo_reducido, entrada_reducida, 'LineWidth', 2)
title('Triangular entrada')
xlabel('Tiempo [s]')
ylabel('Amplitud [Norm]')
grid on

%%% REPRESENTACIÓN ESPECTRAL DE LA SEÑAL DE ENTRADA -> SUMA DE TRES TONOS
espectro = fft(entrada_normalizada);
P2 = 20.*log10(abs(espectro./1));
P1 = P2(1:N/2+1);
%P1(2:end-1) = 2*P1(2:end-1);
f = fs*(0:(N/2))/N;

figure();
plot(f,P1,'LineWidth',2)
title('Triangular entrada')
xlabel('Frecuencia [Hz]');
ylabel('Ampitud [dB]');
grid on


%%% SEÑAL FILTRADA -> FILTRO PASO ALTO 
%%% fs = 10 kHz , %%% fc = 750 Hz
%%% Filtro FIR -> Ventana Hamming -> Orden: 60

tiempo = linspace(0,N*Ts,N); %Eje de tiempo
filtrada = filtrada'; %Al importar la columna con los datos de la señal filtrada, el vector lo llamo "filtrada"
maximo_filtrada = max(filtrada);
filtrada_normalizada = filtrada./maximo_filtrada;
%%% REPRESENTACIÓN TEMPORAL DE LA SEÑAL FILTRADA -> FILTRO PASO ALTO
plot(tiempo,filtrada_normalizada,'LineWidth',2)
title('Triangular Filtrada')
xlabel('Tiempo [s]');
ylabel('Ampitud [Norm]');
grid on

%%% REPRESENTACIÓN ESPECTRAL DE LA SEÑAL SEÑAL FILTRADA -> FILTRO PASO ALTO
espectro_filtrado = fft(filtrada_normalizada);
P2_filtrado = 20.*log10(abs(espectro_filtrado./1));
P1_filtrado = P2_filtrado(1:N/2+1);
f_filtrada = fs*(0:(N/2))/N;

figure();
plot(f_filtrada,P1_filtrado,'LineWidth',2)
title('Triangular Filtrada')
xlabel('Frecuencia [Hz]');
ylabel('Ampitud [dB]');
grid on


