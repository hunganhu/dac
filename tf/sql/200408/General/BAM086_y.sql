USE TF_KHJ
GO

drop table y_BAM085_200408
go
create table y_BAM085_200408 (IDN Char(6), max_bucket float)
go

insert into y_BAM085_200408
select IDN, max(bucket)
from y_BAM085_tmp_200408
WHERE ACCOUNT_CODE = 'Y'
group by IDN
go

select * from bam085_200408 where account_code = 'Y'
