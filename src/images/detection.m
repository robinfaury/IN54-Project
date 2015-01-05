function detection()

    hcontadj = vision.ContrastAdjuster;
       x = imread('5.jpg');
       x = rgb2gray(x);
       y = step(hcontadj, x);
       subplot(1,2,1); imshow(x); title('Original Image');
       subplot(1,2,2); imshow(y); title('Contrast-adjusted Image');

       hedge = vision.EdgeDetector;
       hcsc = vision.ColorSpaceConverter(...
        'Conversion', 'RGB to intensity');
       hidtypeconv = vision.ImageDataTypeConverter('OutputDataType', 'single');
       img = step(hcsc,x);
       img1 = step(hidtypeconv, img);
       edges = step(hedge, img1);
       imshow(edges);
end