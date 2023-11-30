figure;
clear;
clf;
axis equal;
hold on;
grid on;

block_x=[0 0 40391 40391 0 ];
block_y=[41353 41394 41394 41353 41353 ];
fill(block_x,block_y,'g');
text(20195.5, 41373.5, '4');

block_x=[0 0 40 40 0 ];
block_y=[41 41353 41353 41 41 ];
fill(block_x,block_y,'g');
text(20, 20697, '1');

block_x=[40 40 40351 40351 40 ];
block_y=[41 41353 41353 41 41 ];
fill(block_x,block_y,'w');
text(20195.5, 20697, '2');

block_x=[40351 40351 40391 40391 40351 ];
block_y=[41 41353 41353 41 41 ];
fill(block_x,block_y,'g');
text(40371, 20697, '3');

block_x=[0 0 40391 40391 0 ];
block_y=[0 41 41 0 0 ];
fill(block_x,block_y,'g');
text(20195.5, 20.5, '0');

LINE = line([0 40391],[0 0],'Color','k');
set(LINE, 'linewidth', 3)
LINE = line([0 0],[0 41394],'Color','k');
set(LINE, 'linewidth', 3)
LINE = line([40391 40391],[0 41394],'Color','k');
set(LINE, 'linewidth', 3)
LINE = line([0 40391],[41394 41394],'Color','k');
set(LINE, 'linewidth', 3)
