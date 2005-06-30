USE TF_GX
GO

drop table y_BAM085_200303
go
create table y_BAM085_200303 (IDN Char(6), max_bucket float)
go

insert into y_BAM085_200303
select IDN, max(bucket)
from y_BAM085_tmp_200303
WHERE ACCOUNT_CODE = 'Y'
group by IDN
go

select * from bam085_200303 where account_code = 'y'

