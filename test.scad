/*
Parameters
- wall radius
- wall height
- hexagon radius
- hexagon width
*/

wall_radius = 5;
wall_height = 1;
hexagon_radius = 2;
hexagon_width = 0.2;


wall_width = wall_radius / 50;
eps = 1e-3;
FN = 100;   // number of parts


module hexagon(height, center) {
    outer_radius = hexagon_radius + hexagon_width / 2;
    inner_radius = hexagon_radius - hexagon_width / 2;
    
    translate(center) {
        difference() {
            cylinder(
                h=height,
                r1=outer_radius,
                r2=outer_radius,
                center=true,
                $fn=6   
            );
            cylinder(
                h=height+eps,
                r1=inner_radius,
                r2=inner_radius,
                center=true,
                $fn=6   
            );
        };
    };
}

/*
Let's place the origin at (0,0,0).
The entire tiling can be translated + rotated later
*/
module hexagon_tiling_excess(height) {
    inner_hex_radius = hexagon_radius * sqrt(3) / 2;
    y_max = wall_radius + 2 * hexagon_radius;
    i_max = ceil(y_max / (2 * hexagon_radius));
    
    // some crazy ass math that probably could 
    // be reformatted to be more understandable
    for (yi = [-i_max : 1 : i_max]) {
        y = yi * 2 * inner_hex_radius;
        for (xi = [-i_max : 1 : i_max]) {
            x = xi * 3 * hexagon_radius;
            hexagon(height, [x, y, 0]);
            hexagon(height, [
                x+3/2*hexagon_radius, 
                y+inner_hex_radius, 
                0
            ]);
        }
    }
}

module hexagon_tiling(height, origin, rot) {
    intersection() {
        inner_cylinder();
        rotate(a=rot, v=[0,0,1]) {
            translate(origin) {
                hexagon_tiling_excess(height);
            };
        };
    }
}

module outer_cylinder()
{
    cylinder(
        h=wall_height,
        r1=wall_radius + wall_width,
        r2=wall_radius + wall_width,
        center=true,
        $fn=FN
    );
}

module inner_cylinder()
{
    cylinder(
        h=wall_height+eps,
        r1=wall_radius,
        r2=wall_radius,
        center=true,
        $fn=FN
    );
}

module cylinder_wall()
{
    difference() {
        outer_cylinder();
        inner_cylinder();
    }
}


/*
Put everything together now and clap your hands!
*/
module yeet() 
{
    cylinder_wall();
    cyl_height = wall_height / 3;
    O_mid = [0,0,0];
    O_low = [-hexagon_radius/3, hexagon_radius/4, cyl_height];
    O_high = [2*hexagon_radius/3, -hexagon_radius/4, -cyl_height];
    hexagon_tiling(cyl_height, O_mid, 0);
    hexagon_tiling(cyl_height, O_low, 20);
    hexagon_tiling(cyl_height, O_high, 40);
}
yeet();


