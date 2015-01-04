function detection()

 hedge = vision.EdgeDetector;
       hcsc = vision.ColorSpaceConverter(...
        'Conversion', 'RGB to intensity');
       hidtypeconv = vision.ImageDataTypeConverter('OutputDataType', 'single');
       img = step(hcsc, imread('0.jpg'));
       img1 = step(hidtypeconv, img);
       edges = step(hedge, img1);
       imshow(edges);
end