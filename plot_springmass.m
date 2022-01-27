load('springmass.txt');

hold on 
mass1 = plot(springmass(1, 1), springmass(1, 2), 'o');
mass2 = plot(springmass(1, 3), springmass(1, 4), 'o');
xlim([-1, 1])
ylim([-1, 1])
axis square

for i = 2:size(springmass, 1)
    mass1.XData = springmass(i, 1);
    mass1.YData = springmass(i, 2);
    mass2.XData = springmass(i, 3);
    mass2.YData = springmass(i, 4);
    drawnow
end