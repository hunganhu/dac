 update vars
 set pd = case
   when score < -0.0570 then 0.0001
   when score < -0.0477 then 0.0001
   when score < -0.0417 then 0.0001
   when score < -0.0376 then 0.0001
   when score < -0.0358 then 0.0001
   when score < -0.0336 then 0.0001
   when score < -0.0306 then 0.0001
   when score < -0.0281 then 0.0001
   when score < -0.0263 then 0.0001
   when score < -0.0248 then 0.0001
   when score < -0.0219 then 0.0001
   when score < -0.0186 then 0.0001
   when score < -0.0159 then 0.0001
   when score < -0.0139 then 0.0001
   when score < -0.0111 then 0.0001
   when score < -0.0094 then 0.0001
   when score < -0.0078 then 0.0001
   when score < -0.0066 then 0.000113576
   when score < -0.0049 then 0.000135677
   when score < -0.0033 then 0.000160571
   when score < -0.0024 then 0.00018844
   when score < -0.0009 then 0.000219468
   when score < 0.0008 then 0.000253841
   when score < 0.0023 then 0.00029158
   when score < 0.0044 then 0.000333026
   when score < 0.0062 then 0.000378537
   when score < 0.0068 then 0.000428152
   when score < 0.0076 then 0.000482066
   when score < 0.0093 then 0.000540475
   when score < 0.0105 then 0.000603576
   when score < 0.0127 then 0.000671272
   when score < 0.0148 then 0.000744037
   when score < 0.0169 then 0.000822365
   when score < 0.0179 then 0.000906185
   when score < 0.0200 then 0.000995701
   when score < 0.0224 then 0.001091117
   when score < 0.0242 then 0.001192639
   when score < 0.0266 then 0.001300476
   when score < 0.0296 then 0.001414346
   when score < 0.0321 then 0.00153492
   when score < 0.0337 then 0.001662895
   when score < 0.0351 then 0.001798023
   when score < 0.0366 then 0.001940515
   when score < 0.0392 then 0.002090586
   when score < 0.0420 then 0.00224845
   when score < 0.0448 then 0.002414324
   when score < 0.0472 then 0.002587687
   when score < 0.0502 then 0.002769459
   when score < 0.0530 then 0.002960592
   when score < 0.0559 then 0.003160606
   when score < 0.0596 then 0.003369719
   when score < 0.0626 then 0.003588154
   when score < 0.0654 then 0.003816133
   when score < 0.0692 then 0.00405288
   when score < 0.0728 then 0.004299577
   when score < 0.0763 then 0.004557446
   when score < 0.0796 then 0.004825754
   when score < 0.0830 then 0.005104729
   when score < 0.0864 then 0.005394597
   when score < 0.0894 then 0.005695588
   when score < 0.0923 then 0.006007931
   when score < 0.0954 then 0.006330504
   when score < 0.0987 then 0.006664844
   when score < 0.1027 then 0.007012539
   when score < 0.1059 then 0.00737252
   when score < 0.1098 then 0.00774503
   when score < 0.1131 then 0.008130314
   when score < 0.1167 then 0.008528627
   when score < 0.1203 then 0.008940235
   when score < 0.1244 then 0.009363649
   when score < 0.1278 then 0.009800884
   when score < 0.1307 then 0.010254046
   when score < 0.1336 then 0.010721804
   when score < 0.1371 then 0.011204607
   when score < 0.1404 then 0.011703004
   when score < 0.1444 then 0.012217679
   when score < 0.1482 then 0.012747292
   when score < 0.1526 then 0.013295135
   when score < 0.1570 then 0.013864957
   when score < 0.1626 then 0.01445669
   when score < 0.1678 then 0.015073165
   when score < 0.1733 then 0.015718255
   when score < 0.1801 then 0.016397282
   when score < 0.1885 then 0.017117561
   when score < 0.1952 then 0.017885783
   when score < 0.2022 then 0.01871876
   when score < 0.2101 then 0.019638641
   when score < 0.2203 then 0.020668063
   when score < 0.2331 then 0.021842097
   when score < 0.2459 then 0.023208399
   when score < 0.2600 then 0.024831525
   when score < 0.2747 then 0.026798665
   when score < 0.2924 then 0.02921509
   when score < 0.3117 then 0.0322472
   when score < 0.3333 then 0.036116939
   when score < 0.3664 then 0.041095352
   when score < 0.4094 then 0.047555906
   when score < 0.4703 then 0.055994279
   when score < 0.5526 then 0.067176549
   else 0.121825618
 end
 where flag = 12;
 
 update vars
 set pd = case
   when score < -0.0248 then 0.00051
   when score < -0.0033 then 0.000580557
   when score < 0.0105 then 0.000773013
   when score < 0.0321 then 0.00114939
   when score < 0.0559 then 0.001773999
   when score < 0.0894 then 0.002719687
   when score < 0.1244 then 0.004085285
   when score < 0.1626 then 0.006035933
   when score < 0.2459 then 0.008884946
   else 0.013244816
 end
 where flag = 11;
 
 update vars
 set pd = 0.0001
 where flag = 10;
