disp("magcal");

close("all");

function fitp = dofit(angle, mag, label)
	fitp = polyfit(angle, mag, 5);
	figure();
	plot(angle, mag, angle, polyval(fitp, angle));
	title(label);
	xlabel("Angle");
	ylabel("Offset");
endfunction

function [fitx, fity] = domag(filename)
	magdata = load("-ascii", filename);

	[rows, _] = size(magdata);
	angle = magdata(:, 1);
	magx = magdata(:, 2);
	magy = magdata(:, 3);

	fitx = dofit(angle, magx, [filename, " - x"]);
	fity = dofit(angle, magy, [filename, " - y"]);

	disp(filename);
	disp("fitx: "); disp(fitx);
	disp("fity: "); disp(fity);
endfunction

function [xoffset, yoffset, yscale] = dospin(magxoffset, magyoffset)
	spindata = load("-ascii", "spin.txt");
	magx = spindata(:, 1) - magxoffset;
	magy = spindata(:, 2) - magyoffset;

	xoffset = (max(magx) + min(magx)) / 2;
	yoffset = (max(magy) + min(magy)) / 2;
	yscale = (max(magx) - min(magx)) / (max(magy) - min(magy));
	disp("spin.txt");
	disp("xoffset: "); disp(xoffset);
	disp("yoffset: "); disp(yoffset);
	disp("yscale: "); disp(yscale);

	figure();
	plot(magx - xoffset, (magy - yoffset)*yscale);
	title("spin");
endfunction

[x0p, y0p] = domag("mag0.txt");
[x1p, y1p] = domag("mag1.txt");
[x2p, y2p] = domag("mag2.txt");
dospin(polyval(x0p+x1p+x2p, pi/2), polyval(y0p+y1p+y2p, pi/2)); # spin test is done with wheels facing forward, pi/2

