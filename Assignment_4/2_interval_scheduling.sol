Problem:    2_interval_scheduling
Rows:       9
Columns:    4 (4 integer, 4 binary)
Non-zeros:  19
Status:     INTEGER OPTIMAL
Objective:  z = 3 (MAXimum)

   No.   Row name        Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 z                           3                             
     2 a0                          0                           1 
     3 a1                          2                           2 
     4 a2                          2                           2 
     5 a3                          2                           2 
     6 a4                          1                           2 
     7 a5                          2                           2 
     8 a6                          2                           2 
     9 a7                          1                           2 

   No. Column name       Activity     Lower bound   Upper bound
------ ------------    ------------- ------------- -------------
     1 x1           *              1             0             1 
     2 x2           *              1             0             1 
     3 x3           *              0             0             1 
     4 x4           *              1             0             1 

Integer feasibility conditions:

KKT.PE: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

KKT.PB: max.abs.err = 0.00e+00 on row 0
        max.rel.err = 0.00e+00 on row 0
        High quality

End of output
