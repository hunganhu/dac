USE TF_GX
GO

drop table y_BAM085_200312
go
create table y_BAM085_200312 (IDN Char(6), max_bucket float)
go

insert into y_BAM085_200312
select IDN, max(bucket)
from y_BAM085_tmp_200312
WHERE ACCOUNT_CODE = 'Y'
group by IDN
go

select * from bam085_200312 where account_code = 'y'

