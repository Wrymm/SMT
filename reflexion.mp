% Reflexion macro package
% Authors: Markus Holzer and Stefan Schwoon.

%%%% Various definitions %%%%

numeric unit;
unit=5/9cm;

color c_wall,c_grid,c_dshaded,c_mirror,c_diamond,c_path;
c_wall=0.3white;
c_grid=0.93white;
c_dshaded=0.7white;
c_mirror=0.8blue;
c_diamond=0.7red;
c_path=0.7green;

unitsquare:=unitsquare shifted (-1/2*(1,1));

path mirrorpath,diampath;
mirrorpath=(-0.4,-0.1)--(0.4,-0.1)--(0.4,0.1)--(-0.4,0.1)--cycle;
diampath=(0.3,0.1)--(0.1,0.3)--(-0.1,0.3)--(-0.3,0.1)--(-0.3,-0.1)--(-0.1,-0.3)--(0.1,-0.3)--(0.3,-0.1)--cycle;

numeric last_wall,last_mirror,last_diamond;
numeric startx, starty, targetx, targety, bpset;
numeric wallx[],wally[],mirrorx[],mirrory[],mirrora[],diamondx[],diamondy[];
path ballpath;
last_wall=0;
last_mirror=0;
last_diamond=0;
startx=0; starty=0;
targetx=0; targety=0;
bpset=0;

%%%% Functions for defining walls, diamonds, mirrors, etc %%%%

def wall (expr xi,yi) =
	i:=incr(last_wall);
	wallx[i]:=xi; wally[i]:=-yi;
enddef;

vardef start(expr x,y) = startx:=x; starty:=-y; enddef;
vardef target(expr x,y) = targetx:=x; targety:=-y; enddef;

def mirror(expr xi,yi,oi) =
	i:=incr(last_mirror);
	mirrorx[i]:=xi;mirrory[i]:=-yi;
	if (oi="/"): mirrora[i]:=45; fi;
	if (oi="\"): mirrora[i]:=315; fi;
enddef;

def diamond(expr xi,yi) =
	i:=incr(last_diamond);
	diamondx[i]:=xi; diamondy[i]:=-yi;
enddef;

def move_ball(expr p) =
	bpset:=1;
	ballpath:=p;
enddef;

%%%% Function for drawing the puzzle %%%%

numeric xmin,ymin,xmax,ymax;

vardef draw_grid (expr xi,yi,lx,ly) = 
	for i=0 upto lx:
		draw ((xi+i-1/2,yi-1/2)--(xi+i-1/2,yi+ly-1/2)) scaled unit withcolor c_grid;
	endfor;
	for i=0 upto ly:
		draw ((xi-1/2,yi+i-1/2)--(xi+lx-1/2,yi+i-1/2)) scaled unit withcolor c_grid;
	endfor;
enddef;

vardef draw_all =
	beginfig(0);
	if (bpset=1):
        draw ballpath scaled unit reflectedabout ((0,0),(1,0)) dashed evenly withcolor c_path;
	fi;

	for i=1 upto last_wall:
		fill (unitsquare shifted (wallx[i],wally[i])) scaled unit withcolor c_wall;
	endfor;
	fill (unitsquare shifted (startx,starty)) scaled unit withcolor c_dshaded;
	fill (unitsquare shifted (targetx,targety)) scaled unit withcolor c_dshaded;
	xmin:=wallx[1]; xmax:=wallx[1];
	ymin:=wally[1]; ymax:=wally[1];
	for i=1 upto last_wall:
		if (wallx[i] < xmin): xmin:=wallx[i]; fi;
		if (wally[i] < ymin): ymin:=wally[i]; fi;
		if (wallx[i] > xmax): xmax:=wallx[i]; fi;
		if (wally[i] > ymax): ymax:=wally[i]; fi;
	endfor;
	draw_grid(xmin,ymin,xmax-xmin+1,ymax-ymin+1);
	label(btex $S$ etex scaled 1.5,(startx,starty) scaled unit);
	label(btex $E$ etex scaled 1.5,(targetx,targety) scaled unit);
        for i=1 upto last_mirror:
	    fill mirrorpath rotated mirrora[i] shifted (mirrorx[i],mirrory[i])
			scaled unit withcolor c_mirror;
	    draw mirrorpath rotated mirrora[i] shifted (mirrorx[i],mirrory[i])
			scaled unit;
        endfor;
        for i=1 upto last_diamond:
	    fill diampath shifted (diamondx[i],diamondy[i])
			scaled unit withcolor c_diamond;
	    draw diampath shifted (diamondx[i],diamondy[i]) scaled unit;
        endfor;

	endfig;
enddef;
