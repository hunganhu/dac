 update vars
 set pd = case  
   when score < -0.057034608 then 0.001938448
   when score < -0.047663603 then 0.001947737
   when score < -0.041741703 then 0.001953197
   when score < -0.037596317 then 0.001957094
   when score < -0.035824264 then 0.001960152
   when score < -0.033553188 then 0.001962705
   when score < -0.030595447 then 0.001964948
   when score < -0.028129256 then 0.001967015
   when score < -0.02628125 then 0.001969009
   when score < -0.024835967 then 0.001971025
   when score < -0.021948689 then 0.00197315
   when score < -0.018550831 then 0.001975473
   when score < -0.015939097 then 0.001978088
   when score < -0.013918483 then 0.001981094
   when score < -0.01108814 then 0.001984598
   when score < -0.00944862 then 0.001988715
   when score < -0.00784303 then 0.00199357
   when score < -0.006621971 then 0.001999299
   when score < -0.004904442 then 0.002006047
   when score < -0.003320335 then 0.002013974
   when score < -0.002407873 then 0.002023249
   when score < -0.000943402 then 0.002034053
   when score < 0.000752156 then 0.002046582
   when score < 0.002268637 then 0.002061045
   when score < 0.004363683 then 0.002077663
   when score < 0.00623 then 0.002096673
   when score < 0.00684875 then 0.002118324
   when score < 0.007603379 then 0.00214288
   when score < 0.00930619 then 0.002170622
   when score < 0.010543778 then 0.002201845
   when score < 0.012715281 then 0.002236858
   when score < 0.014779882 then 0.002275988
   when score < 0.016857127 then 0.002319576
   when score < 0.01787875 then 0.002367981
   when score < 0.019969511 then 0.002421578
   when score < 0.022350454 then 0.002480758
   when score < 0.024183188 then 0.00254593
   when score < 0.026612495 then 0.00261752
   when score < 0.029558269 then 0.002695972
   when score < 0.03212591 then 0.002781747
   when score < 0.033697262 then 0.002875325
   when score < 0.035056948 then 0.002977203
   when score < 0.036590419 then 0.003087897
   when score < 0.03918875 then 0.003207943
   when score < 0.04198191 then 0.003337895
   when score < 0.044788292 then 0.003478324
   when score < 0.047214732 then 0.003629825
   when score < 0.050249675 then 0.003793008
   when score < 0.052987473 then 0.003968507
   when score < 0.055862769 then 0.004156972
   when score < 0.05958082 then 0.004359076
   when score < 0.062643647 then 0.004575512
   when score < 0.06538511 then 0.004806993
   when score < 0.069151318 then 0.005054253
   when score < 0.072845213 then 0.005318047
   when score < 0.076289134 then 0.005599151
   when score < 0.079560136 then 0.005898365
   when score < 0.082955393 then 0.006216506
   when score < 0.086410845 then 0.006554417
   when score < 0.089405635 then 0.006912962
   when score < 0.092288244 then 0.007293025
   when score < 0.095414567 then 0.007695514
   when score < 0.098741537 then 0.008121361
   when score < 0.102729625 then 0.008571519
   when score < 0.105916749 then 0.009046965
   when score < 0.109775932 then 0.009548697
   when score < 0.113090299 then 0.010077739
   when score < 0.116740021 then 0.010635137
   when score < 0.120316755 then 0.011221961
   when score < 0.124420408 then 0.011839305
   when score < 0.127751303 then 0.012488288
   when score < 0.130653323 then 0.013170051
   when score < 0.133649799 then 0.013885761
   when score < 0.137136783 then 0.014636609
   when score < 0.140425735 then 0.015423811
   when score < 0.144412026 then 0.016248607
   when score < 0.148241465 then 0.017112264
   when score < 0.152602781 then 0.018016072
   when score < 0.157009176 then 0.018961348
   when score < 0.162645205 then 0.019949434
   when score < 0.16784349 then 0.020981698
   when score < 0.173271523 then 0.022059533
   when score < 0.180120723 then 0.023184359
   when score < 0.188527723 then 0.024357621
   when score < 0.195231985 then 0.025580793
   when score < 0.202223881 then 0.026855374
   when score < 0.21011642 then 0.028182896
   when score < 0.220316862 then 0.029564931
   when score < 0.233099491 then 0.031003128
   when score < 0.245858163 then 0.032499339
   when score < 0.25996386 then 0.034056037
   when score < 0.274743627 then 0.03567772
   when score < 0.29238773 then 0.037375543
   when score < 0.311749973 then 0.039182401
   when score < 0.333268507 then 0.041201421
   when score < 0.366398163 then 0.043759956
   when score < 0.409409465 then 0.047892691
   when score < 0.470269444 then 0.056839271
   when score < 0.552599989 then 0.080633035
   else 0.150001
 end
 where flag = 12;
 
 update vars
 set pd = case
   when score < -0.024835967 then 0.00051
   when score < -0.003320335 then 0.000580557
   when score < 0.010543778 then 0.000773013
   when score < 0.03212591 then 0.00114939
   when score < 0.055862769 then 0.001773999
   when score < 0.089405635 then 0.002719687
   when score < 0.124420408 then 0.004085285
   when score < 0.162645205 then 0.006035933
   when score < 0.245858163 then 0.008884946
   else 0.013244816   
 end
 where flag = 11;
 
 update vars
 set pd = 0.0001
 where flag = 10;
 
 update vars
 set risk_level = case
   When pd < 0.007293025 then 'L'
   When pd < 0.043759956 then 'M'
   else 'H'
 end
 where flag in (10, 11, 12);
