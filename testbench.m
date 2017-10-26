% Run this with octave
function testbench()

frontier = load('pareto.txt');
all = load('pareto_data.txt');

% Sort frontier elements by X coordinate, so we get a nice line.
frontier = sortrows(frontier);

plot( ...
	all(:,1), all(:,2), 'o', ...
	frontier(:,1), frontier(:,2), '-*' ...
);
