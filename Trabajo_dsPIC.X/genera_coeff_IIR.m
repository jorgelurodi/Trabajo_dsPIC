% SCRIPT de MATLAB para generar archivos ".s" para la implementación de filtros IIR.
% Este script llama al archivo FPB_IIR.m que contiene una función que devuelve
% una estructura con los coeficientes del filtro calculados.

close all
clear all

% Variable auxiliar igual al nombre del archivo generado por la fdatool
aux = FPB_IIR; % Asegúrate de que esta función devuelve un objeto dfilt.df2sos

% Obtener las secciones de segundo orden
sosMatrix = aux.sosMatrix;

% Calcular el número de coeficientes del numerador (b) y denominador (a)
num_coef_sos = size(sosMatrix, 1); % Número de secciones SOS
num_coef_b = num_coef_sos * 3; % Cada sección SOS tiene 3 coeficientes del numerador
num_coef_a = num_coef_sos * 3; % Cada sección SOS tiene 3 coeficientes del denominador

% Calcular la cantidad de memoria a reservar para los coeficientes y muestras
c_mem_reserv = 0;
while (2^c_mem_reserv) < max(num_coef_b, num_coef_a)
    c_mem_reserv = c_mem_reserv + 1;
end
mem_reserv = 2^(c_mem_reserv + 1);

% Convertir los coeficientes a formato fixed-point I1Q15
sign = 1;
word_length = 16;
fraction_length = 15;

fract_b_coef = fi(zeros(1, num_coef_b), sign, word_length, fraction_length);
fract_a_coef = fi(zeros(1, num_coef_a), sign, word_length, fraction_length);

idx = 1;
for i = 1:num_coef_sos
    for j = 1:3
        fract_b_coef(idx) = fi(sosMatrix(i, j), sign, word_length, fraction_length);
        fract_a_coef(idx) = fi(sosMatrix(i, j+3), sign, word_length, fraction_length);
        idx = idx + 1;
    end
end

% Mostrar los coeficientes en formato hexadecimal
disp('Numerador (b) coeficientes:');
disp(fract_b_coef);
hex_b_coef = arrayfun(@(x) x.hex, fract_b_coef, 'UniformOutput', false);
disp(hex_b_coef);

disp('Denominador (a) coeficientes:');
disp(fract_a_coef);
hex_a_coef = arrayfun(@(x) x.hex, fract_a_coef, 'UniformOutput', false);
disp(hex_a_coef);

% Establecer los paths para guardar el archivo .s generado
path = 'C:\Users\jorge\Documents\Trabajo_dsPIC\Trabajo_dsPIC.X\';
file_name = input('Nombre para fichero IIR (sin el .s):', 's');
file = strcat(file_name, '.s');
tm = menu('Escoge el tipo de mem. para coef.', 'X Data space', 'Program Space');
fid = fopen(strcat(path, file), 'w');

% Escribir en el archivo .s
fprintf(fid, ';.....\n;    File   %s \n;.....\n\n', file);
fprintf(fid, '    .equ %sNumTapsB, %u\n', file_name, num_coef_b);
fprintf(fid, '    .equ %sNumTapsA, %u\n\n', file_name, num_coef_a);
fprintf(fid, ';.....\n; Allocate and initialize filter taps\n\n');
if (tm == 1)
    fprintf(fid, '        .section .xdata\n');
else
    fprintf(fid, '        .section .%sconst, "x"\n', file_name);
end
fprintf(fid, '        .align %u\n', mem_reserv);

% Escribir coeficientes del numerador (b)
fprintf(fid, '%sTapsB:', file_name);
for cc = 1:num_coef_b
    if (rem(cc, 9) == 1)
        fprintf(fid, '\n.hword ');
    end
    if ((rem(cc, 9) == 0) || (cc == num_coef_b))
        fprintf(fid, '0x%s ', hex_b_coef{cc});
    else    
        fprintf(fid, '0x%s, ', hex_b_coef{cc});
    end    
end

% Escribir coeficientes del denominador (a)
fprintf(fid, '\n\n%sTapsA:', file_name);
for cc = 1:num_coef_a
    if (rem(cc, 9) == 1)
        fprintf(fid, '\n.hword ');
    end
    if ((rem(cc, 9) == 0) || (cc == num_coef_a))
        fprintf(fid, '0x%s ', hex_a_coef{cc});
    else    
        fprintf(fid, '0x%s, ', hex_a_coef{cc});
    end    
end

fprintf(fid, '\n\n;.....\n; Allocate delay line in (uninitialized) Y data space\n\n');
fprintf(fid, '        .section .ybss,  "b"\n');
fprintf(fid, '        .align %u\n\n', mem_reserv);
fprintf(fid, '%sDelay:\n', file_name);
fprintf(fid, '        .space (%sNumTapsB+%sNumTapsA)*2\n\n', file_name, file_name);
fprintf(fid, '; .....\n; Allocate and initialize filter structure\n\n');
fprintf(fid, '        .section .data\n');
fprintf(fid, '        .global _%sFilter\n\n', file_name);
fprintf(fid, '_%sFilter:\n', file_name);
fprintf(fid, '.hword %sNumTapsB\n', file_name);
fprintf(fid, '.hword %sNumTapsA\n', file_name);
if (tm == 1)
    fprintf(fid, '.hword %sTapsB\n', file_name);
    fprintf(fid, '.hword %sTapsB+%sNumTapsB*2-1\n', file_name, file_name);
    fprintf(fid, '.hword %sTapsA\n', file_name);
    fprintf(fid, '.hword %sTapsA+%sNumTapsA*2-1\n', file_name, file_name);
    fprintf(fid, '.hword 0xff00\n');
else
    fprintf(fid, '.hword psvoffset(%sTapsB)\n', file_name);
    fprintf(fid, '.hword psvoffset(%sTapsB)+%sNumTapsB*2-1\n', file_name, file_name);
    fprintf(fid, '.hword psvoffset(%sTapsA)\n', file_name);
    fprintf(fid, '.hword psvoffset(%sTapsA)+%sNumTapsA*2-1\n', file_name, file_name);
    fprintf(fid, '.hword psvpage(%sTapsB)\n', file_name);
end   
fprintf(fid, '.hword %sDelay\n', file_name);
fprintf(fid, '.hword %sDelay+(%sNumTapsB+%sNumTapsA)*2-1\n', file_name, file_name, file_name);
fprintf(fid, '.hword %sDelay\n', file_name);
fprintf(fid, '\n; ....................................\n; ....................................\n');
fclose(fid);





