f = imread('pic.jpg');
figure(1);
imshow(f);
title('original image');

PQ = paddedsize(size(f));
[U,V] = dftuv(PQ(1),PQ(2));
 F = fftshift(fft2(f,PQ(1),PQ(2)));

 S =  abs(F);
 ag = angle(F);

[l,p] = size(S);
d = max(max(S));
c = min(min(S));

for l = 1:1:l
    for p = 1:1:p
        S1(l,p) = (S(l,p)-c)*(255/(d-c));
    end
end

figure(2);
subplot(2,2,1);
imshow(uint8(S1));
title('Magnitude response without log transform');

% since dc dominates the values of spectrume, the dynamic range of other intensities in displayed image is compression. to bring out those details we perform log transformation.

[m,n] = size(ag);
d = max(max(ag));
c = min(min(ag));

for l = 1:1:l  
    for p = 1:1:p
        ag1(l,p) = (ag(l,p)-c)*(255/(d-c));
    end
end

subplot(2,2,2);
imshow(uint8(ag1));
title('Phase response without log transform');
