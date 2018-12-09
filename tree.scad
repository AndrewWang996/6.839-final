

threshold = 0.08;
angle = atan2(4,3);

module tree(size) {
    if (size < threshold) {
        // nothing
    }
    else {
        square(size=[size, size], center=true);
        
        adj_size = size * cos(angle);
        adj_origin = [
            -size/2 + adj_size/sqrt(2) * cos(45+angle),
            size/2 + adj_size/sqrt(2) * sin(45+angle),
            0
        ];
        adj_rotate = angle;
        translate(adj_origin)
            rotate(a=adj_rotate, v=[0,0,1])
                tree(adj_size);
        
        
        
        opp_size = size * sin(angle);
        opp_origin = [
            size/2 - opp_size/sqrt(2) * cos(135-angle),
            size/2 + opp_size/sqrt(2) * sin(135-angle),
            0
        ];
        opp_rotate = angle - 90;
        
        
        
        translate(opp_origin)
            rotate(a=opp_rotate, v=[0,0,1])
                tree(opp_size);
        
        
    }
}

tree(2);







