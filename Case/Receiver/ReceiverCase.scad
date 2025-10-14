include <BOSL2/std.scad>

//cube([140,120,2]);

difference() {
    cube([140,120,30]);
    up(2) right(2) back(2) cube([136,116,28]);
    #back(120/2) up(30/2) xrot(90) yrot(90) cylinder(h=2, d=7);
    #back(70) up(10) cube([2,15,10]);
}
