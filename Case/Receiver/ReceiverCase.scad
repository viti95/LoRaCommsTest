include <BOSL2/std.scad>

difference(){
    union() {
        cube([140,120,2]);
        up(2) back(2) right(2) cube([136,2,4]);
        up(2) back(116) right(2) cube([136,2,4]);
        up(2) right(2) back(2) cube([2,116,4]);
        up(2) right(136) back(2) cube([2,116,4]);
    };
    #right (2+15) back((120-98)/2) cube([41,98,2]);
    //#back((120-65)/5) right(140-10-4-63) cube([63,65,2]);
    #back(41) right(92) grid_copies(n=[8,8], spacing=[3+5,3.5+5])
    cube([5,5,2]);
}

/*difference() {
    cube([140,120,30]);
    up(2) right(2) back(2) cube([136,116,28]);
    #back(120/2) up(30/2) xrot(90) yrot(90) cylinder(h=2, d=7);
    #back(70) up(5) cube([2,20,20]);
}*/
