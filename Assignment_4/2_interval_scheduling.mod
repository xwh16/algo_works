var x1, binary;
var x2, binary;
var x3, binary;
var x4, binary;

maximize z: x1 + x2 + x3 + x4;

s.t. a0: 1 <= 2;
s.t. a1: x1 + x2 <= 2;
s.t. a2: x1 + x2 + x3 <= 2;
s.t. a3: x1 + x2 + x3 <= 2;
s.t. a4: x2 + x3 <= 2;
s.t. a5: x2 + x4 <= 2;
s.t. a6: x2 + x4 <= 2;
s.t. a7: x4 <= 2;

end;
