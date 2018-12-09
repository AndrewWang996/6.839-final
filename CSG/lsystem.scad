/* L-system OpenSCAD library by Hans Loeblich
This library is for creating L-systems, aka Lindenmayer System, 
which is a kind of formal grammar that can be used to generate space-filling curves 
and other fractal shapes by applying replacement rules recursively.
See https://en.wikipedia.org/wiki/L-system for a better, more detailed explanation.
Many of the example curves in this file use rules found on that wikipedia page.
This library relies on two main functions for generating the fractal paths:
1) function L_system(rules, start, n) 
  This is used to generate a string of symbols by recursively applying the system rules.
2) function create_path(instrs, angle=90, pos=[0,0], heading=0)
  This interprets the string returned by L_system as a simple kind of "turtle graphics"
  instructions, which simulates the path of a turtle that can move forward and turn.
The "turtle graphics" of create_path supports the following operations:
  "F" : moves the turtle "forward" by one unit
  "-" : rotates the turtle to the left, by the given angle (default 90)
  "+" : rotates the turtle to the right, by the given angle (default 90)
  Any other character encountered will be ignored by create_path.
  Due to the create_path implementation, computation time increases with the square of the instruction length.
  And instruction length is multiplied by 2 to 16 times every iteration(or even more depending on the specific rules)
  So increasing n can drastically multiply the amount of time required.
  Some recommended absolute maximum n values have been given for each curve, 
  which compute in less than a minute or so my workstation.
  
  You can try higher n values but it may use a lot of memory(and swap) and time to compute.
  The "tree" option is used to choose between a tree-like nested list structure 
  as opposed to a flat list that takes n^2 time to build.  It is a tradeoff of 
  memory consumption vs time.  Tree is faster but uses more RAM.
  All the generated shapes are currently 2D, and its recommended to **USE F6 RENDER** to view each curve.  
  I think it takes roughly the same time to complete as F5, but the framerate of moving the camera etc. 
  will be much better without OpenCSG rendering a 2D object as 3D on these complex models.
  Update 2/24/2018 Recent improvements to OpenSCAD(check the dev nightly builds) have greatly reduced memory usage for highly recursive scripts. 
    The "tree" implementation option appears to be faster than the non-tree in most if not all cases now.
*/

$fn=12;
rounded = true; // Add circles at each vertex
// setting rounded = false might be slightly faster to preview/render 
// but gives an uglier curve path


//dragon_curve(n=14, tree=true);         // Recommended n <= 15
//dragon_curve(n=14, angle=72, tree=true); // "pentagonal dragon"
//dragon_curve(n=14, angle=60, tree=true); // hexagonal?

//twin_dragon(n=13, tree=true);          // Recommended n <= 13

terdragon(n=10, tree=true);            // Recommended n <= 10

//hilbert_curve(n=7, w=0.5, tree=true);  // Recommended n <= 7 

//moore_curve(n=5, w=0.5, tree=true);    // Recommended n <= 6 

//peano_curve(n=5, tree=true);           // Recommended n <= 5

//gosper_curve(n=5, tree=true);          // Recommended n <= 5

//levy_c_curve(n=13, tree=true);         // Recommended n <= 13

//koch_curve(n=8, tree=true);            // Recommended n <= 6
  
//koch_snowflake(n=6, tree=true);        // Recommended n <= 6

//quadratic_type1_koch(n=5, tree=true);  // Recommended n <= 6

//quadratic_type2_koch(n=5, tree=true);  // Recommended n <= 6

//sierpinski_triangle(n=8, tree=true);   // Recommended n <= 8

//sierpinski_arrowhead(n=8, tree=true); // Recommended n <= 8



module dragon_curve(n=10, angle=90, w=0.4, tree=false) {
  L_system_lines([["X", "X+YF+"],["Y","-FX-Y"]], "FX", n, angle, w, tree=tree);
}

module twin_dragon(n=10, angle=90, w=0.4, tree=true) {
  L_system_lines([["X", "X+YF"],["Y","FX-Y"]], "FX+FX+", n, angle, w, tree=tree);
}

module terdragon(n=10, angle=120, w=0.4, tree=true) {
  L_system_lines([["F", "F+F-F"]], "F", n, angle, w, tree=tree);
}

module hilbert_curve(n=4, angle=90, w=0.4, tree=false) {
  L_system_lines([["A", "-BF+AFA+FB-"],["B","+AF-BFB-FA+"]], "A", n, angle, w, tree=tree);
}

module moore_curve(n=4, angle=90, w=0.4, tree=false) {
  L_system_lines( [["L", "-RF+LFL+FR-"], ["R","+LF-RFR-FL+"]], "LFL+F+LFL", n, angle, w, tree=tree);
}

module peano_curve(n=3, angle=90, w=0.4, tree=false) {
  L_system_lines([["L", "LFRFL-F-RFLFR+F+LFRFL"], ["R", "RFLFR+F+LFRFL-F-RFLFR"]], "L", n, angle, w, tree=tree);
}

module gosper_curve(n=4, angle=60, w=0.4, tree=false) {
  // final pass, convert A and B to (F)orward instructions
  final_pass_rules = [["A","F"], ["B","F"]];
  L_system_lines([["A", "A-B--B+A++AA+B-"], ["B", "+A-BB--B-A++A+B"]], "A", n, angle, w, final_pass_rules, tree=tree);
}

module levy_c_curve(n=10, angle=45, w=0.4, tree=false) {
  L_system_lines([["F", "+F--F+"]], "F", n, angle, w, tree=tree);
} 

module koch_curve(n=4, angle=60, w=0.4, tree=false) {
  L_system_lines([["F", "F-F++F-F"]], "F", n, angle, w, tree=tree);
} 

module koch_snowflake(n=4, angle=60) {
  L_system_polygon([["F", "F-F++F-F"]], "F++F++F", n, angle);
} 

module quadratic_type1_koch(n=4, angle=90, w=0.4, tree=true) {
  L_system_lines([["F", "F-F+F+F-F"]], "F", n, angle, w, tree=tree);
} 

module quadratic_type2_koch(n=4, angle=90, w=0.4, tree=true) {
  L_system_lines([["F", "F-F+F+FF-F-F+F"]], "F", n, angle, w, tree=tree);
} 

module sierpinski_triangle(n=5, angle=120, w=0.4, tree=true) {
  // for drawing purposes, G is replaced with F
  final_pass_rules = [["G","F"]]; 
  L_system_lines([["F", "F-G+F+G-F"], ["G", "GG"]], "F-G-G", n, angle, w, final_pass_rules, tree=tree);
}

module sierpinski_arrowhead(n=6, angle=60, w=0.4) {
  L_system_lines([["X", "YF+XF+Y"], ["Y", "XF-YF-X"]], "XF", n, angle, w);
}




module L_system_lines(rules, start, n, angle=90, w=0.4, final_pass_rules, tree=true) {
    if (tree) {
      echo("Tree");
      instrs = L_system(rules, start, n, final_pass_rules, tree);
      //echo(instrs);
      tree_path = create_tree_path(instrs, angle);
      //echo(tree_path);
      dfs_segmented_line(tree_path, w);
      if (rounded) translate(last_2dpoint(tree_path)) circle(d=w); 
    } else {
      echo("No Tree");
      instrs = L_system(rules, start, n, final_pass_rules, tree);
      //echo(instrs);
      path = create_path(instrs, angle);
      //echo(path);
      segmented_line(path, w);
    }
}

module L_system_polygon(rules, start, n, angle=90, final_pass_rules) {
  instrs = L_system(rules, start, n, final_pass_rules, false);
  path = create_path(instrs, angle);
  //echo(path);
  polygon(path);
}

// traverse a tree of 2d points, depth first, drawing lines between them
module dfs_segmented_line(root, w, prev, i=0) {
  l = len(root);
  if (l > 0) {
    if (i < l) {
      curr = root[i];
      if (is_2dpoint(curr)) {
        if (prev != undef) {
          //echo(prev, " to ", curr);
          line(prev, curr, w); 
        }
      } else {
        //newlast = last_2dpoint(second);
        dfs_segmented_line(curr, w, prev, 0);
      }
      dfs_segmented_line(root, w, last_2dpoint(curr), i+1);
    }
  }
}

// works on long lists of points
module segmented_line(points, w=0.1) {
  // OpenSCAD doesn't allow ranges > 10000 in modules, 
  // so limit is used to split very large segmented lines into manageable chunks
  limit = 9999; 
  l = len(points);
  imax = floor((l-2) / limit);
  for (i = [0:1:imax]) {
    jmin = i*limit;
    jmax = min(jmin + limit - 1, (l-2));
    for (j = [jmin:1:jmax]) {
      line(points[j],points[j+1], w); 
    }
  }
  if (rounded) translate(points[l]) circle(d=w); 
}

module line(A, B, w=0.1) {
  xa = A[0];
  ya = A[1];
  xb = B[0];
  yb = B[1];
  dx = (xb - xa);
  dy = (yb - ya);
  d = sqrt(dx*dx + dy*dy);
  a = atan2(dy, dx);
  translate(A) {
    if (rounded) circle(d=w);
    rotate([0,0,a]) translate([0,-w/2]) square([d,w]);
  }
}

function is_string(x) = str(x)==x;
function is_char(x) = x == x[0]; // works except for undef
function is_number(x) = x[0]==undef;
function is_2dpoint(p) = (len(p) == 2) && is_number(p.x) && is_number(p.y);
function is_2dpoint_list(list) = _is_2dpoint_list(list, len(list));
function _is_2dpoint_list(list, i) = is_2dpoint(list[i]) ? ((i==0) ? true : _is_2dpoint_list(list, i-1)) : false;

// traverse tree last elements until 2d point is found
function last_2dpoint(root) = is_2dpoint(root) ? root : last_2dpoint(root[len(root)-1]);

// Take a list of strings and return the concatenation of all strings
function join(strs, i=0) = (i == len(strs)-1 ? strs[i] : str(strs[i], join(strs, i+1)) );
 
// take a string, and a second string representing all valid characters.
// return only the valid characters from the first string
function filter_string(string, valids) = 
  let(imax = len(string)-1, jmax = len(valids)-1)
  join([for (i = [0:imax], j = [0:jmax]) let (ch = string[i]) if (ch == valids[j]) ch]);

// wrapper for the recursive function, formats the final pass rules for efficient usage
function L_system(rules, start, n, final_pass_rules=[], tree=true) = 
  let(
    // string of all chars that are allowed as drawing instructions
    valid_chars = "F-+", 
    imax = len(rules)-1,
    jmax = len(final_pass_rules)-1,
    final_rules = [for (rule = rules) 
        let(
          r_val = rule[1],
          kmax = len(r_val),
          // run final pass rules over original rules
          new_r_val = (len(final_pass_rules) > 0) ? 
            join([for (k = [0:kmax])
              let(ch = r_val[k], results = quiet_search(ch, final_pass_rules, 0, 0) ) 
              len(results) == 0 ? ch : final_pass_rules[results[0]][1]]) :
            r_val
        )
        [rule[0], filter_string(new_r_val, valid_chars)]
    ]
  )
  tree ?
    _L_system_tree(rules, start, n, final_rules) :
    _L_system(rules, start, n, final_rules);

// recursive function that applies replacement rules
// keeping data in a tree like structure helps guarantee that max recursion limit is not reached
function _L_system_tree(rules, val, n, final_rules) = 
  let(imax = len(val)-1)
  n == 0 ? val : 
    [for (i = [0:imax]) 
      let(
        ch = val[i],
        rs = (n == 1) ? final_rules : rules, 
        results = quiet_search(ch, rs, 0, 0) ) 
      len(results) == 0 ? 
        ch :
        [_L_system_tree(rules, rs[results[0]][1], n-1, final_rules)]
    ];

// Iterate an L-system, recursively apply replacement rules to the string, n times
function _L_system(rules, val, n, final_rules) = 
  let(imax = len(val)-1)
  n == 0 ? val :
    join([
      for (i = [0:imax]) 
        let(
          ch = val[i],    
          rs = (n == 1) ? final_rules : rules, 
          results = quiet_search(ch, rs, 0, 0) ) 
        len(results) == 0 ? 
          ch : // no matching rule, character is a constant
          _L_system(rules, rs[results[0]][1], n-1, final_rules)
    ]);

// Take "turtle instructions" and convert into a list of points on a curve
// this is the user-friendly wrapper for the recursive function
function create_path(instrs, angle=90, pos=[0,0], heading=0) =
  let(
    imax = len(instrs)-1,
    // strip any extraneous characters that are not used as drawing instructions 
    // before calling the recursive function, in order to reduce the i^2 load of concat for every element
    filtered = [for (i = [0:imax], valid = ["F","-","+"]) let (instr = instrs[i]) if (instr == valid) instr],
    l = len(filtered)
  ) 
  _create_path(filtered, l, angle, pos, heading);


// thanks to InPhase for helping re-write this to behave better under heavy recursion
function _create_path(instrs, leninstrs, angle, pos, heading, i=0, prepend=[]) =
  (i >= leninstrs) ?
    [ for (a=[[pos], prepend]) for (b = a) b ] :
    _create_path(instrs, leninstrs, angle, 
      (instrs[i] == "F") ? pos + [cos(heading), sin(heading)] : pos,
      (instrs[i] == "-") ? heading + angle : (instrs[i] == "+") ? heading - angle : heading,
      i+1,
      (instrs[i] == "F") ? concat([pos], prepend) : prepend);

function create_tree_path(root, angle=90, pos=[0,0], heading=0) =
  let( result = _create_tree_path(root, angle, len(root), 0, [pos, heading, [] ])) 
  [pos,result[2]];

// Due to how L_system_tree generates its output, and how create_tree_path processes it,
// max recursion depth is roughly on the order of (n + m),
// where n is number of iterations of the system and m is length of the longest rule for a given system
function _create_tree_path(root, angle, l, i=0, vars) =
  let(
    pos = vars[0],
    heading = vars[1],
    points = vars[2] 
  )
  // pos, heading, points=[0,0]
  (i < l ? 
    let(node = root[i]) 
    (is_char(node) ? 
      // Found character
      (node == "F" ? 
        let(newpos = pos + [cos(heading), sin(heading)])
        _create_tree_path(root, angle, l, i+1, [newpos, heading, concat(points, [newpos])] ) :
        (node == "-" ? 
          _create_tree_path(root, angle, l, i+1, [pos, heading+angle, points]) :
          (node == "+" ?
            _create_tree_path(root, angle, l, i+1, [pos, heading-angle, points]) :
            _create_tree_path(root, angle, l, i+1, vars) // node is unused character, ignore and go to next index
          )
        )
      ) : 
      let( next = _create_tree_path(node, angle, len(node), 0, [pos, heading, []]) )
      // node is not a character, so must be a list, go deeper
      _create_tree_path(root, angle, l, i+1, [next[0], next[1], concat(points, [next[2]])] )
    ) : 
    // i >= l
    vars
  );

// hack to avoid warning messages every time the search function misses
function quiet_search(m, s, n, i) = 
  let(
    t = (i == undef) ? 
      search(m, str(s,m), 0)[0] : 
      search(m, concat(s, [[for(j=[0:1:i]) m ]]), 0, i)[0],
    maxi = (n == 0) ? len(t)-2 : min(len(t)-2, n)
  )
len(t) > 1 ? [ for (i=[0:1:maxi]) t[i] ] : [];
