/* Calculate_Month_Since  */
 update source
 set month_since =
   (cast (substring(statement_month,1,4) as int) - 1911) * 12 +
    cast (substring(statement_month,5,2) as int)

