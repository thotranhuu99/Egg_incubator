A = uint8([0 1; 0 1]);
B = uint8([0 0; 1 1]);
TTable = bitxor(A, B);
data = [uint8(190) uint8(239)];
polynomial = uint8(49);
polynomial_bin = bitget(polynomial,8:-1:1,'uint8');
crc_initial = uint8(255);
crc_bin = bitget(crc_initial,8:-1:1,'uint8');
 for i=1:2
    data_bin = bitget(data(i),8:-1:1,'uint8');
    crc_bin = bitxor(data_bin, crc_bin, 'uint8');
    for j=1:8
        if (crc_bin(1)==0)
            for k=1:7
                crc_bin(k) = crc_bin(k+1);
            end
            crc_bin(8) = 0;
        else
            for k=1:7
                crc_bin(k) = crc_bin(k+1);
            end
            crc_bin(8) = 0;
            crc_bin = bitxor(crc_bin,polynomial_bin);
        end
    end
 end
crc = 0;
for i=1:8
    crc = crc + crc_bin(i)*2^(8-i);
end
crc_byte =uint8(crc)
