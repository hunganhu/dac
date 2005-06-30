
create view bam085_200303 as
select * from tf_GX.dbo.BAM085_200303


create table bam085_bucket_200303 (
  idn char(14),
  bank_code char(7),
  mon_since  int,
  bucket     float

)


 declare month_cur CURSOR local scroll static for
    select idn, data_yyy, data_mm, bank_code, rtrim(pay_code_12)
      from bam085_200303

 declare @idn char(14)
 declare @data_yyy char(5)
 declare @data_mm  char(2)
 declare @bank_code char(7)
 declare @pay_code_12 char(12)
 declare @paycode char(1)
 declare @bucket  float
 declare @month_since int
 declare @cycle int
 declare @i int

 open month_cur
 fetch next from month_cur into @idn, @data_yyy, @data_mm, @bank_code, @pay_code_12

 while (@@fetch_status = 0)
    begin
       set @month_since = @data_yyy * 12 + @data_mm
       set @cycle = len(@pay_code_12)
       set @i = 0

       while  @i < @cycle
          begin
             set @paycode = substring(@pay_code_12, (@i + 1), 1) 
             set @bucket = (case when @paycode = 'A' then 0.25
                                 when @paycode = 'B' then 0.5
                                 when @paycode = 'X' then 0
                                 else convert(float, @paycode) end)  
             insert into bam085_bucket_200305 (idn, bank_code, mon_since, bucket)
                values (@idn, @bank_code, @month_since - @i, @bucket)
             set @i = @i + 1
          end
       fetch next from month_cur into @idn, @data_yyy, @data_mm, @bank_code, @pay_code_12
    end
 close month_cur
 deallocate month_cur

/*

select * from oliver.bam085_200303 order by idn, bank_code
select * from oliver.bam085_bucket_200303 order by idn, bank_code, mon_since

select sum(cycles) from oliver.bam085_200303




*/