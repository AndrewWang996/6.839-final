$fn = 100;
difference() {
cube([10,5,5], center=true);
rotate([90,0,0]) translate([0,-2.00,0]) cylinder(10,4.00,4.00, center=true);
}
